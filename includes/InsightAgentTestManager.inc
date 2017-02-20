<?php

/**
 * @file
 * Contains InsightAgentTestManager class.
 */

/**
 * Class InsightAgentTestManager.
 */
class InsightAgentTestManager {

  /**
   * Basic factory function for report plugins.
   */
  public static function factory($plugin) {
    // Verify existence for handler class.
    if (!class_exists($plugin['handler']['class'])) {
      // The plugin handler class is defined but it can't be found.
      $message = 'Class <em>!name</em> is not exists!';
      watchdog('insight_agent', $message, ['!name' => $plugin['handler']['class']], WATCHDOG_ERROR);
      return FALSE;
    }

    return new $plugin['handler']['class']($plugin);
  }

}
