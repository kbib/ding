<?php

/**
 * Alma user settings form.
 */
function ding_user_admin_settings_form($form_state) {
  $form = array(
    // We need to set the submit handler explicitly, because the 
    // #submit array is appended to by system_settings_form.
    '#submit' => array('ding_user_admin_settings_form_submit'),
  );

  $form['ding_user_secure_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Secure roles'),
    '#description' => t('Select the roles that are high security, requiring username/password login instead the simple Alma patron login. Note that the superuser account (uid=1) is always considered to have a secure role.'),
    '#options' => user_roles(TRUE),
    '#default_value' => variable_get('ding_user_secure_roles', array()),
  );

  return system_settings_form($form);
}

/**
 * Submit handler for user settings form.
 */
function ding_user_admin_settings_form_submit($form, &$form_state) {
  // Removed unchecked (value == 0) values from the secure roles array.
  // This makes it possible to use a simple intersect to figure out if 
  // the user has a secure role.
  foreach (array_keys($form_state['values']['ding_user_secure_roles'], 0) as $key) {
    unset($form_state['values']['ding_user_secure_roles'][$key]);
  }
}

