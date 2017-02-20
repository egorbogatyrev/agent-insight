<?php

/**
 * @file
 * Contains InsightAgentTestSuit interface.
 */

/**
 * Class InsightAgentTestSuit.
 */
abstract class InsightAgentTestSuit implements InsightAgentTestInterface {

  /**
   * The plugin definition for this instance.
   */
  protected $plugin;

  /**
   * Constructor for the report plugin class.
   */
  public function __construct($plugin) {
    $this->plugin = $plugin;
  }

  /**
   * {@inheritdoc}
   */
  public function getId() {
    return $this->plugin['handler']['class'];
  }

  /**
   * {@inheritdoc}
   */
  public function getName() {
    return $this->plugin['name'];
  }

  /**
   * Loads the test cases.
   */
  abstract public function loadTestCases($full = FALSE);

}
