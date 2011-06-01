<?php
// $Id$
/**
 * @file similar_events.inc
 * Provides a content type that shows similar events.
 */

/**
 * Callback function to supply a list of content types.
 *
 * This is only required to make CTools know that we're providing
 * multiple types here (since we have no single => TRUE in the array).
 */
function ding_event_similar_events_ctools_content_types() {
  return array(
    'title' => t('Node'),
  );
}

/**
 * Callback function to supply a list of content types.
 */
function ding_event_similar_events_content_type_content_types() {
  $types = array(
    'similar_events_title' => array(
      'title' => t('Similar events'),
      'description' => t('Provides a display of similar events.'),
    ),
  );

  // Append our default settings to every type.
  foreach ($types as $name => $type) {
    $types[$name] += array(
      'category' => t('Node'),
      'required context' => new ctools_context_required(t('Node'), 'node'),
    );
  }
  return $types;
}

/**
 * Return the pane contents.
 */
function ding_event_similar_events_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;
  $events = ding_event_get_similar($node);
  if ($events) {
    $block->content = theme('ding_event_similar_events', $events);
  }
  return $block;
}

/**
 * Return the pane titles.
 */
function ding_event_similar_events_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" similar events', array('@s' => $context->identifier));
}

/**
 * Returns an edit form for the content type.
 *
 * We're not actually doing anything here, but we need to have this hook
 * for this content type to work.
 */
function ding_event_similar_events_content_type_edit_form(&$form, &$form_state) {
  return $form;
}

