<?php // $Id$

/**
 * @file ding_library_map.admin.inc
 * Admin forms
 */

/**
 * The admin settings form.
 */
function ding_library_map_admin_settings_form(&$form_state) {
  $form = array();

  $form['ding_library_map']['ding_library_map_center'] = array(
    '#type' => 'textfield',
    '#title' => t('Map center'),
    '#description' => t('The default center coordinates of the map, expressed as a decimal latitude and longitude, separated by a comma. Leave blank to use Gmap module default.'),
    '#default_value' => variable_get('ding_library_map_center', ''),
  );
  
  return system_settings_form($form);
}