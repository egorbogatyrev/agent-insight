<?php

/**
 * @file
 * Contains InsightAgentTestCase class.
 */

/**
 * Class InsightAgentTestCase.
 */
abstract class InsightAgentTestCase implements InsightAgentTestInterface {

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
   * Runs the test case and returns the results.
   */
  abstract public function runTestCase();

}
