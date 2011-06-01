<?php // $Id$

/**
 * @file ding_library.views.inc
 * Views-hooks for the ding_library module.
 */

/**
 * Implementation of hook_views_data().
 *
 * Describe our table structure to Views.
 */
function ding_library_views_data() {
  $data = array();

  // The group name for our table fields.
  $data['ding_library']['table']['group'] = t('Ding library');

  // Declare that our table is related to the node table.
  $data['ding_library']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['ding_library']['library_page_type'] = array(
    'title' => t('Library page type'),
    'help' => t('The page type selected for this library'),
    // TODO: Check if there's a better handler for fields with a fixed 
    // set of options.
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['ding_library']['library_weight'] = array(
    'title' => t('Weight'),
    'help' => t('The weight is used to determine the sorting order of the libraries.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['ding_library']['library_show_on_map'] = array(
    'title' => t('Show on map'),
    'help' => t('This setting controls whether libraries are shown on maps pages, etc.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Show on map'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  return $data;
}

