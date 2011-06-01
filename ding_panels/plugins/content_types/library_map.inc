<?php
// $Id$

/**
 * @file object.inc
 * Provides a Panels content type for display of Ting objects.
 */

/**
 * Callback function to supply a list of content types.
 */
function ding_panels_library_map_ctools_content_types() {
  return array(
    'title' => t('Library map'),
    'single' => TRUE,
    'description' => t('Provides a map of all our libraries.'),
    'category' => t('Library page'),
  );
}

/**
 * Return the pane contents.
 */
function ding_panels_library_map_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();

  if (function_exists('ding_library_map_display')) {
    $block->content = ding_library_map_display();
  }

  return $block;
}

/**
 * Return the pane titles.
 */
function ding_panels_library_map_content_type_admin_title($subtype, $conf, $context) {
  return t('Ding library map');
}

/**
 * Returns an edit form for the content type.
 *
 * We're not actually doing anything here, but we need to have this hook 
 * for this content type to work.
 */
function ding_panels_library_map_content_type_edit_form(&$form, &$form_state) {
  return $form;
}

