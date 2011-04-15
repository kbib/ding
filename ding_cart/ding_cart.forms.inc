<?php

/**
 * User cart details form.
 *
 * @param array $form_state
 *    Drupal form API state.
 * @param stdClass $account
 *    The user object for the cart we're looking at.
 * @return array
 */
function ding_cart_list_form(&$form_state, $account) {
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['cart'] = array(
    '#type' => 'value',
    '#value' => array(),
  );

  // Fetch the cart data from the database.
  $query = db_query("SELECT * FROM {ding_cart} WHERE uid = %d;", $account->uid);
  $options = array();
  while ($row = db_fetch_array($query)) {
    $form['cart']['#value'][$row['local_id']] = $row;
    $options[$row['local_id']] = '';
  }

  $form['selected'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
  );

  $form['remove'] = array(
    '#type' => 'submit',
    '#value' => t('Remove selected'),
    '#attributes' => array('class' => 'remove-button'),
    '#weight' => 10,
    '#op' => 'remove',
    '#submit' => array('ding_cart_list_form_delete_submit')
  );

  return $form;
}

/**
 * Validation for cart details form.
 */
function ding_cart_list_form_validate($form, &$form_state) {
  $selected_items = array_keys(array_filter($form_state['values']['selected']));

  if (empty($selected_items)) {
    form_set_error('items', t('Please select one or more items from the cart.'));
  }
  $form_state['values']['selected_items'] = $selected_items;
}

/**
 * Submit handler for cart details form.
 */
function ding_cart_list_form_delete_submit($form, &$form_state) {
  $args = array($form_state['values']['account']->uid);
  $args = array_merge($args, $form_state['values']['selected_items']);
  db_query("DELETE FROM {ding_cart} WHERE uid = %d AND local_id IN (" . db_placeholders($form_state['values']['selected_items']) . ")", $args);
}

