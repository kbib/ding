<?php

/**
 * @file ding_library.admin.inc
 * Admin forms, page callbacks and related functions.
 *
 * Much of this stuff is a taken from the scaffolding example module.
 * http://cvs.drupal.org/viewvc.py/drupal/contributions/docs/developer/examples/scaffolding_example/
 */

/**
 * The main settings form.
 */
function ding_content_admin_settings_form(&$form_state) {
  $form = array();

  $vocabularies = array();
  $vocabularies[] = '';
  foreach (taxonomy_get_vocabularies() as $vocabulary) {
    $vocabularies[$vocabulary->vid] =  $vocabulary->name;
  }

  $form['ding_similarterms_vocabulary_id'] = array(
    '#title' => t('Similar terms vocabulary'), 
    '#description' => t('Select which vocabulary to use when determining related content'),
    '#type' => 'select',
    '#options' => $vocabularies,
    '#default_value' => variable_get('ding_similarterms_vocabulary_id', 0),
  );

  return system_settings_form($form);
}

