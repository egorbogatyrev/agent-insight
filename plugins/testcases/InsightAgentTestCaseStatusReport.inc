<?php

/**
 * @file
 * Contains InsightAgentTestCaseStatusReport class which used to
 * get information about status reports.
 */

$plugin = array(
  'name' => 'Status Report',
  'description' => 'Provides the status report important.',
  'handler' => array(
    'class' => 'InsightAgentTestCaseStatusReport',
  ),
);

/**
 * Class InsightAgentTestCaseStatusReport.
 */
class InsightAgentTestCaseStatusReport extends InsightAgentTestCase {

  /**
   * {@inheritdoc}
   */
  public function runTestCase() {
    return [
      'label' => $this->getName(),
      'result' => '8 min ago',
      'status' => 'WARNING',
      'url' => 'http://site.com/admin/config/media/file-system',
    ];
  }

}
