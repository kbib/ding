<?php
/**
 * @file
 * Page callbacks for Ding cart module.
 */

/**
 * Page callback to handle adding stuff to the cart.
 *
 * Handles both AJAX and regular requests, presenting the user with the
 * opportunity to log in if needed.
 */
function ding_cart_page_add_to_cart($item_id) {
  global $user;
  module_load_include('client.inc', 'ting');
  $is_ahah = ding_cart_detect_ahah();
  $object = ting_get_object_by_local_id($item_id, TRUE);

  // First, let's check if this is in the cart already :)
  $cart_item_exists = (boolean) db_result(db_query("SELECT created FROM {ding_cart} WHERE uid = %d AND local_id = '%s';", $user->uid, $object->localId));

  // If client wants JSON (usually AJAX requests), let's give it to them.
  if ($is_ahah) {
    $reply = array();
    if (!$user->uid || !ding_provider_invoke('user', 'logged in', $user)) {
      $reply['status'] = 'error';
      $reply['error_type'] = 'not-logged-in';
      $reply['title'] = t('Not logged in');
      $reply['message'] = t('You have to log in to put books in your cart');
    }
    elseif ($cart_item_exists) {
      $reply['status'] = 'error';
      $reply['error_type'] = 'already-in-cart';
      $reply['title'] = t('Already in cart');
      $reply['message'] = t('@item is already in your cart.', array('@item' => $object->title));
    }
    else {
      if (ding_cart_add_ting_object($object) == SAVED_NEW) {
        $reply['status'] = 'success';
        $reply['title'] = t('Added to cart');
        $reply['message'] = t('@item is now in your cart.', array('@item' => $object->title));
        $reply['cart_link'] = url('user/' . $user->uid . '/cart');
      }
      else {
        $reply['status'] = 'error';
        $reply['error_type'] = 'cart-save-error';
        $reply['title'] = t('Cart saving error');
        $reply['message'] = t('An error occurred while saving to the cart. Please try again.');
      }
    }

    // We are returning JavaScript, so tell the browser.
    drupal_set_header('Content-Type: application/json; charset=utf-8');
    echo json_encode($reply);
    return;
  }

  // For regular requests, we present forms, etc.
  if ($cart_item_exists) {
    drupal_set_message(t('@item is already in your cart.', array('@item' => $object->title)) , 'warning');
    return drupal_goto('user/' . $user->uid . '/cart');
  }

  // Present a confirmation form.
  return drupal_get_form('ding_cart_confirm_form', $user, $object);
}

/**
 * Ding cart confirmation form.
 *
 * @param array $form_state
 *    Drupal form API state.
 * @param stdClass $account
 *    Drupal user account.
 * @param TingClientObject $object
 *    The object we're acting on.
 */
function ding_cart_confirm_form(&$form_state, $account, $object) {
  // Send some form values along for the submit step.
  $form = array();

  $form['account'] = array(
    '#type' => 'value',
    '#value' => $account,
  );

  $form['object'] = array(
    '#type' => 'value',
    '#value' => $object,
  );

  $form['dest_url'] = array(
    '#type' => 'value',
    '#value' => $urls[$action],
  );

  return confirm_form($form, t('Add %book to your cart?', array('%book' => $object->title)), 'user/' . $account->uid . '/cart', '');
}

/**
 * Submit handler for Ding cart confirmation form.
 */
function ding_cart_confirm_form_submit($form, &$form_state) {
  // Save the item to the cart table.
  ding_cart_add_ting_object($form_state['values']['object']);
  $form_state['redirect'] = $form_state['values']['dest_url'];
}

