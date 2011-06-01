<?php

// $Id$

/**
 * @file
 * Ding branch plugin.
 */

$plugin = array(
  'description' => t('Example branch plugin'),
  'version' => 1,
);

/**
 * Get library branch list.
 * @return
 *   Keyed array of id => branch names.
 */
function ding_provider_example_branch_list() {
  return array();
}
