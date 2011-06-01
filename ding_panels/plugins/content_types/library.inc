<?php
// $Id$

/**
 * @file library.inc
 * Provides several library related panels content types.
 */


/**
 * Callback function to supply a list of content types.
 *
 * This is only required to make CTools know that we're providing 
 * multiple types here (since we have no single => TRUE in the array).
 */
function ding_panels_library_ctools_content_types() {
  return array(
    'title' => t('Library page'),
  );
}

/**
 * Callback function to supply a list of content types.
 */
function ding_panels_library_content_type_content_types() {
  $types = array(
    'library_location' => array(
      'title' => t('Library location'),
      'description' => t('Shows the address, map and other locative information for the library.'),
    ),
    'library_hours' => array(
      'title' => t('Library opening hours'),
      'description' => t('Shows the library opening hours.'),
    ),
    'library_feature_list' => array(
      'title' => t('Library feature list'),
      'description' => t('Simple list of library features, containing only the title.'),
    ),
    'library_feature_detail_list' => array(
      'title' => t('Library detailed feature list'),
      'description' => t('More detailed view of library features with pictures and description.'),
    ),
    'library_pc_booking' => array(
      'title' => t('Library pc booking'),
      'description' => t('Provide access to booking system for public PC\'s in the library.'),
    ),
  );

  // Append our default settings to every type.
  foreach ($types as $name => $type) {
    $types[$name] += array(
      'category' => t('Library page'),
      'required context' => new ctools_context_required(t('Node'), 'node'),
    );
  }
  return $types;
}

/**
 * Return the pane contents.
 */
function ding_panels_library_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;
  switch($subtype) {
    case 'library_location':
      $block->content = theme('ding_panels_content_library_location', $node);
      break;
    case 'library_hours':
      $block->title = t('Opening hours');
      // Ask CCK to render the opening hours field for us.
      $block->content = content_format('field_opening_hours', $node->field_opening_hours, 'default', $node);
      break;
    case 'library_feature_list':
      // This is a bit of a hack, but currently I haven't been able to
      // find a proper way to set the breadcrumbs on the library front
      // page, so we're calling this function from a content type that's
      // only shown on that page.
      ding_breadcrumbs_set_library($node);
    case 'library_feature_detail_list':
      $block->title = t('Services and facilities');
      $block->content = theme('item_list', _ding_panels_library_features_by_library($node, $subtype));
      break;
    case 'library_pc_booking':
      $block->content = theme('ding_panels_content_library_pc_booking', $node);
      break;
  }
  return $block;
}

/**
 * Return the pane titles.
 */
function ding_panels_library_content_type_admin_title($subtype, $conf, $context) {
  switch($subtype) {
    case 'library_location':
      return t('"@s":  Library location', array('@s' => $context->identifier));
      return t('Library location');
    case 'library_hours':
      return t('"@s":  Library opening hours', array('@s' => $context->identifier));
    case 'library_feature_list':
      return t('"@s":  Library features', array('@s' => $context->identifier));
    case 'library_feature_detail_list':
      return t('"@s":  Library feature details', array('@s' => $context->identifier));
    case 'library_pc_booking':
      return t('"@s":  Library PC booking', array('@s' => $context->identifier));
  }
}

/**
 * Returns an edit form for the content type.
 */
function ding_panels_library_content_type_edit_form(&$form, &$form_state) {
  return $form;
}

/**
 * Find all features related to a library node.
 *
 * @param object $node
 *    The library node we want features for.
 * @param boolean $subtype
 *    Which Panels pane subtype we're rendering.
 * @return array
 *    Array of feature titles, keyed by node id.
 */
function _ding_panels_library_features_by_library($node, $subtype = '') {
  // First, get the feature with a nice bit of SQL.
  $query = db_query("
    SELECT n.nid, n.title
    FROM {content_field_feature_ref} AS fr
    INNER JOIN {node} AS n ON (n.nid = fr.field_feature_ref_nid)
    WHERE n.type = 'feature' AND fr.nid = %d
    ORDER BY fr.delta ASC;
  ", $node->nid);

  // And stick them in a nice little array.
  $features = array();
  if ($subtype == 'library_feature_detail_list') {
    while ($row = db_fetch_array($query)) {
      $node = node_load($row['nid']);
      $features[$row['nid']] = node_view($node, TRUE);
    }
  }
  else {
    while ($row = db_fetch_array($query)) {
      $features[$row['nid']] = l($row['title'], 'node/' . $row['nid']);
    }
  }
  return $features;
}

