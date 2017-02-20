<?php

/**
 * @file
 * Includes admin functions and callbacks.
 */

/**
 * Configuration page for Insight Agent settings.
 */
function insight_agent_configuration_page() {
  $form = [];
  $plugin_suits = insight_agent_get_reports_plugins();

  $form['secretkey'] = array(
    '#type' => 'textfield',
    '#field_prefix' => '<b>' . t('Secret Key') . '</b>',
    '#size' => 32,
    '#description' => t('Key is used for checking access for requests.'),
    '#value' => md5(drupal_get_hash_salt()),
    '#disabled' => TRUE,
  );

  $form['insight_agent_trusted_host'] = array(
    '#type' => 'textfield',
    '#field_prefix' => '<b>' . t('Trusted host') . '</b>',
    '#description' => t('Only from this host Insight Agent will accept requests.'),
    '#default_value' => variable_get('insight_agent_trusted_host', ''),
    '#element_validate' => ['elements_validate_url'],
  );

  foreach ($plugin_suits as $value) {
    /** @var InsightAgentTestSuit $suit */
    $suit = InsightAgentTestManager::factory($value);
    if (!$suit) {
      continue;
    }

    $form_key = $suit->getId();
    $form[$form_key] = array(
      '#type' => 'fieldset',
      '#title' => $suit->getName(),
      '#description' => $value['description'],
      '#collapsible' => TRUE,
    );

    $options = [];
    $plugin_cases = insight_agent_get_reports_plugins('testcases');

    foreach ($plugin_cases as $plugin) {
      /** @var InsightAgentTestCase $case */
      $case = InsightAgentTestManager::factory($plugin);
      if (!$case) {
        continue;
      }
      $options[$case->getId()] = $case->getName();
    }

    $form[$form_key][$form_key] = [
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => variable_get($suit->getId(), []),
    ];
  }

  return system_settings_form($form);
}

/**
 * Access callback for GET request from Insight Server.
 *
 * @return bool
 *   TRUE - in case secret key and params are valid, otherwise - FALSE.
 */
function insight_agent_access_callback() {
  $params = drupal_get_query_parameters();
  if (empty($params['secretKey']) || empty($params['testSuits'])) {
    return FALSE;
  }

  // Verification host.
  $trusted_host = variable_get('insight_agent_trusted_host', '');
  $host = !empty($_SERVER['REMOTE_HOST']) ? $_SERVER['REMOTE_HOST'] : '';
  $host = (!empty($_SERVER['HTTP_REFERER']) && empty($host)) ? $_SERVER['HTTP_REFERER'] : $host;
  if (!empty($host) && !empty($trusted_host)) {
    if (!$host != $trusted_host) {
      return FALSE;
    }
  }

  $salt = md5(drupal_get_hash_salt());
  if ($params['secretKey'] != $salt) {
    drupal_json_output(['response' => 'Access denied']);
    drupal_exit();
  }

  return TRUE;
}

/**
 * Request callback.
 */
function insight_agent_request_callback() {
  global $base_url;
  $plugins = insight_agent_get_reports_plugins();

  // Initialize the response array.
  $output = [
    'host' => $base_url,
    'requestTime' => REQUEST_TIME,
    'responseTime' => REQUEST_TIME,
    'availableTestSuits' => implode(', ', array_keys($plugins)),
    'testSuits' => [],
  ];

  $params = drupal_get_query_parameters();
  $test_suits = $params['testSuits'];
  foreach ($test_suits as $id) {
    if (empty($plugins[$id])) {
      continue;
    }

    /** @var InsightAgentTestSuit $plugin */
    $plugin = InsightAgentTestManager::factory($plugins[$id]);

    /** @var InsightAgentTestCase $case */
    foreach ($plugin->loadTestCases(TRUE) as $case) {
      $output['testSuits'][$plugin->getId()][$case->getId()] = $case->runTestCase();
    }
  }

  drupal_json_output($output);
  drupal_exit();
}

/**
 * Returns insight plugins.
 *
 * @param string $type
 *   Plugin type.
 * @param null|string $id
 *   ID of plugin.
 *
 * @return mixed
 *   Array of plugins.
 */
function insight_agent_get_reports_plugins($type = 'testsuits', $id = NULL) {
  ctools_include('plugins');
  return ctools_get_plugins('insight_agent', $type, $id);
}
