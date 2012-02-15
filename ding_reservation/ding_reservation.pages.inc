<?php
/**
 * @file
 * Page callbacks for Ding reservation module.
 */

/**
 * Reserve an item immediately.
 */
function ding_reservation_page_reserve_item($object, $reservable = NULL) {
  global $user;

  $args = func_get_args();
  if (sizeof($args) == 1) {
    //No id for the reserable item so get it from the Ting object
    $reservable = $object->localId;
  } else {
    //Some reserable items e.g. periodicals have complex ids containing /
    //These are intepreted as seperate arguments for the page callback so
    //combine them.
    $reservable = implode('/', array_slice($args,1));
  }
  
  // If client wants JSON (usually AJAX requests), let's give it to them.
  if (ding_reservation_detect_ahah()) {
    $reply = array();
    $result = ding_provider_invoke('reservation', 'create', $user, $reservable);
    switch ($result['status']) {
      case DING_RESERVATION_SUCCESS:
        $reply['status'] = 'success';
        $reply['title'] = t('Reservation added');
        $reply['message'] = isset($result['branch']) ?
          t('You have reserved @item and will be notified when its ready for pickup at @branch.', array('@item' => $object->title, '@branch' => $result['branch'])) :
          t('You have reserved @item and will be notified when its ready for pickup.', array('@item' => $object->title));
        $reply['cart_link'] = url('user/' . $user->uid . '/status');
        break;
      case DING_PROVIDER_AUTH_REQUIRED:
        $reply['status'] = 'error';
        $reply['error_type'] = 'not-logged-in';
        $reply['title'] = t('Not logged in');
        $reply['message'] = t('You have to log in to make reservations.');
        break;
      case DING_PROVIDER_AUTH_BLOCKED:
        $reply['status'] = 'error';
        $reply['error_type'] = 'account-blocked';
        $reply['title'] = t('Account blocked');
        $reply['message'] = t('Your are not allowed to make reservations, as your account is blocked. Contact your local library to have your account reinstated.');
        break;
      case DING_RESERVATION_ERROR_NO_DEFAULT_BRANCH:
        $reply['status'] = 'error';
        $reply['error_type'] = 'no-preferred-branch';
        $reply['title'] = t('Preferred branch not set');
        $reply['message'] = t('To make quick reservations, you need to have a preferred branch set. You can set one up at your !profile_settings.',
                            array('!profile_settings' => l(t('profile settings page'), 'user/' . $user->uid . '/profile', array('query' => drupal_get_destination()))));
        break;
      case DING_RESERVATION_ERROR_NOT_ALLOWED:
        $reply['status'] = 'error';
        $reply['error_type'] = 'reservation-not-allowed';
        $reply['title'] = t('Not allowed');
        $reply['message'] = t('Reservation of @item is not allowed.', array('@item' => $object->title));
        break;
      case DING_RESERVATION_ERROR_NOT_AVAILABLE:
        $reply['status'] = 'error';
        $reply['error_type'] = 'reservation-not-available';
        $reply['title'] = t('Not available');
        $reply['message'] = t('@item is not available for reservation.', array('@item' => $object->title));
        break;
      case DING_RESERVATION_ERROR_RESERVATION_EXISTS:
        $reply['status'] = 'error';
        $reply['error_type'] = 'reservation-exists';
        $reply['title'] = t('Reservation exists');
        $reply['message'] = t('@item is already reserved.', array('@item' => $object->title));
        break;
      default:
        $reply['status'] = 'error';
        $reply['error_type'] = 'reservation-error';
        $reply['title'] = t('Reservation error');
        $reply['message'] = t('An error occurred while reserving item. Please try again.');
        break;
    }

    // We are returning JavaScript, so tell the browser.
    drupal_set_header('Content-Type: application/json');
    echo json_encode($reply);
    return;
  }

  // Ensure that we're authenticated with the backend, redirect if not.
  ding_library_user_required();
  ting_enrich_object($object);

  if ($reservation_exists && $reservation_exists != DING_PROVIDER_AUTH_REQUIRED) {
    drupal_set_message(t('@item is already reserved.', array('@item' => $object->title)), 'warning');
    return drupal_goto('user/' . $user->uid . '/status');
  }

  return drupal_get_form('ding_reservation_confirm_form', $user, $object, $reservable);
}

/**
 * Ding reservation confirmation form.
 *
 * @param array $form_state
 *    Drupal form API state.
 * @param stdClass $account
 *    Drupal user account.
 * @param TingClientObject $object
 *    The object we're acting on.
 */
function ding_reservation_confirm_form(&$form_state, $account, $object, $reservable) {
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

  $form['reservable'] = array(
    '#type' => 'value',
    '#value' => $reservable,
  );

  $form['dest_url'] = array(
    '#type' => 'value',
    '#value' => $urls[$action],
  );

  return confirm_form($form, t('Reserve %book immediately?', array('%book' => $object->title)), 'user/' . $account->uid . '/status', '');
}

/**
 * Submit handler for Ding cart confirmation form.
 *
 * @todo:
 * Do we really need a confirm form? If we do, it should show
 * reservation options too.
 */
function ding_reservation_confirm_form_submit($form, &$form_state) {
  $reservation = ding_provider_invoke('reservation', 'create', $form_state['values']['account'], $form_state['values']['reservable'], array('pickup_branch' => $form_state['values']['account']->preferred_branch));
  
  if ($reservation && $reservation['status'] == DING_RESERVATION_SUCCESS) {
    drupal_set_message(t('@item is now on your reservation list.', array('@item' => $form_state['values']['object']->title)));
  }
  else {
    drupal_set_message(t('Reservation of @item failed.', array('@item' => $form_state['values']['object']->title)));
  }

  $form_state['redirect'] = $form_state['values']['dest_url'];
}

/**
 * Detect if a request wants a JSON-response, usually for AHAH.
 */
function ding_reservation_detect_ahah() {
  return (isset($_SERVER['HTTP_ACCEPT']) && (strpos($_SERVER['HTTP_ACCEPT'], 'application/json')) || (strpos($_SERVER['HTTP_ACCEPT'], 'text/javascript')));
}
