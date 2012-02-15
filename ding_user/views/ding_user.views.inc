<?php

/**
 * Implementation of hook_views_data()
 */
function ding_user_views_data() {
  $tables['ding_user'] =  array(
    'table' => array(
      'group' => t('User'),
      'join' => array(
        'users' => array(
          'field' => 'uid',
          'left_field' => 'uid'
          )
        ),
      'join' => array(
        'node' => array(
          'field' => 'uid',
          'left_field' => 'uid'
          )
        ),
      ),
    'display_name' => array(
      'title' => t('Ding user display name'), // The item as it appears on the UI,
      'help' => t('The display name for the user.'), // The help that appears on the UI,
      'field' => array(
        'handler' => 'ding_user_field_display_name',
        'click sortable' => TRUE,
        ),
      'sort' => array('handler' => 'views_handler_sort'),
      'argument' => array('handler' => 'views_handler_argument_string'),
      'filter' => array('handler' => 'views_handler_filter_string'),
    ),
  );

  return $tables;
}

/**
 * Implementation of hook_views_data_alter()
 */
function ding_user_views_data_alter(&$data) {
  // Add display names to users
  $data['users']['display_name'] = array(
    'field' => array(
      'title' => t('Ding user display name'),
      'help' => t('The display name for the user.'),
      'handler' => 'ding_user_field_display_name',
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function ding_user_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'ding_user') .'/views',
    ),
    'handlers' => array(
      'ding_user_field_display_name' => array(
        'parent' => 'views_handler_field_user_name',
      ),
    ),
  );
}
