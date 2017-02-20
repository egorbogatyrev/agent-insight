<?php

/**
 * @file
 * Contains InsightAgentTestSuitGeneral class which used to get
 * different information about site depends on test cases.
 */

$plugin = array(
  'name' => 'General test suit',
  'description' => 'Provides the general info about different events.',
  'handler' => array(
    'class' => 'InsightAgentTestSuitGeneral',
  ),
);

/**
 * Class InsightAgentTestSuitGeneral.
 */
class InsightAgentTestSuitGeneral extends InsightAgentTestSuit {

  /**
   * {@inheritdoc}
   */
  public function loadTestCases($full = FALSE) {
    $cases = variable_get($this->getId(), []);
    if (empty($cases)) {
      $this->plugin['test_cases'] = $cases;
    }

    ctools_include('plugins');
    $plugins = ctools_get_plugins('insight_agent', 'testcases');
    foreach ($cases as $id) {
      if (!isset($plugins[$id])) {
        continue;
      }
      $this->plugin['test_cases'][$id] = $full ? InsightAgentTestManager::factory($plugins[$id]) : $plugins[$id];
    }

    return $this->plugin['test_cases'];
  }

}
