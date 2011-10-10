<?php

/**
 * Reservation settings form.
 */
function ding_reservation_admin_settings_form(&$form_state) {
  $form = array();

  $form['ding_reservation_default_interest_period'] = array(
    '#type' => 'select',
    '#title' => t('Default interest period'),
    '#description' => t("Reservation interest period, when the user haven't set one."),
    '#options' => array(
      7 => t('One week'),
      14 => t('Two weeks'),
      30 => t('One month'),
      91 => t('Three months'),
      183 => t('Six months'),
      365 => t('One year'),
    ),
    '#default_value' => variable_get('ding_reservation_default_interest_period', DING_RESERVATION_DEFAULT_INTEREST_PERIOD),
  );

  return system_settings_form($form);
}
