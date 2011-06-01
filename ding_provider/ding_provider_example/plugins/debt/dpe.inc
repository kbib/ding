<?php

// $Id$

/**
 * @file
 * Ding debt plugin.
 */

$plugin = array(
  'description' => t('Example debt plugin'),
  'version' => 1,
);

/**
 * Settings form.
 */
function ding_provider_example_debt_settings_form() {
  $form['description'] = array(
    '#type' => 'markup',
    '#value' => t("Ding Dibs Integration module is required for payments."),
  );

  $form['status'] = array(
    '#type' => 'item',
    '#title' => t('Payments'),
    '#value' => function_exists('ding_dibs_payment_prepare') ? t('Enabled') : t('Disabled'),
  );

  return $form;
}

/**
 * Get a list of debts for a user.
 *
 * @param $account
 *   The Drupal user object.
 * @return array
 *   An array of id => debt, where debt is an array of:
 *   - id: the same id as in the outer array.
 *   - display_title: An description for the user.
 *   - payable: Whether it is payable through Ding.
 *   - date: The date of the debt.
 *   - amount: The amount owed.
 */
function ding_provider_example_debt_list($account) {
}
