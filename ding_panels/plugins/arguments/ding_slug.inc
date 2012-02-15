<?php

/**
 * @file
 * CTools plugin to provide an argument handler for Ding library slugs.
 */

/**
 * Implementation of specially named hook_ctools_arguments().
 */
function ding_panels_ding_slug_ctools_arguments() {
  $args['ding_slug'] = array(
    'title' => t('Ding library slug'),
    'keyword' => 'ding_slug',
    'description' => t('Finds the ding library node context based on the "slug" name for it.'),
    'context' => 'ding_panels_ding_slug_context',
    'placeholder form' => array(
      '#type' => 'textfield',
      '#description' => t('Enter the object ID for this argument'),
    ),
  );
  return $args;
}

/**
 * Discover if this argument gives us the node we crave.
 */
function ding_panels_ding_slug_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  // If unset it wants a generic, unfilled context.
  if ($empty) {
    return ctools_context_create_empty('node');
  }

  // We can accept either a node object or a pure nid.
  if (is_object($arg)) {
    return ctools_context_create('node', $arg);
  }

  // Cast argument to an integer to see if it looks like a node ID.
  $num = intval($arg);
  if (is_numeric($num) && $num > 0) {
    $node = node_load($num);
  }
  // Try looking up the argument as a slug
  elseif (strlen($arg) > 1) {
    $node = ding_library_load_by_slug($arg);
  }
  else {
    return FALSE;
  }

  if (!$node) {
    return FALSE;
  }

  ding_breadcrumbs_set_library($node);
  return ctools_context_create('node', $node);
}

