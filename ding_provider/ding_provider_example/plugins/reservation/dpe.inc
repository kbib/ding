<?php


/**
 * @file
 * Ding reservation plugin.
 */

$plugin = array(
  'description' => t('Example reservation plugin'),
  'version' => 1,
);

/**
 * Get the users reservations.
 *
 * Should only return data readily available, and leave out data that
 * requires extra requests. Implementation can add extra data used by
 * the 'details' function. All the keys described in the return value
 * can be left to the details function to fill in, so the minimum
 * return value can be:
 * @code
 *   array(
 *     123 => array('id' => 123),
 *   );
 * @endcode
 *
 * @param $account
 *   The Drupal user.
 * @return array
 *   An array of id => reservation, where reservation is an array of:
 *   - id: The same id as the key.
 *   - materials_number: Materials number, optional. Is shown to the user.
 *   - ting_object: A TingClientObject instance. Optional. The object
 *     represents the reserved material and provides the basis for displaying
 *     titles, authors etc.
 *   - title: Title of the item. Optional.
 *   - authors: The authors of the item. Optional.
 *   - display_title: Alternative title string, optional. Used if title is
 *     empty. If neither 'title' nor 'display_title' is specified, an
 *     'Title not available' message is displayed.
 *   - status: Status of the reservation, 'active' or 'fetchable'.
 *   - create_date: Creation date, format: YYYY-MM-DD.
 *   - queue_number: For active reservations, number in queue. Optional.
 *   - pickup_number: For fetchable reservations. Optional.
 *   - valid_from: Reservation valid from. Format: YYYY-MM-DD.
 *   - valid_to: Reservation valid to. Format: YYYY-MM-DD.
 *   - pickup_expire_date: For 'fetchable': Expiry of pickup. Optional
 *   - remote_reservation: Whether it is a remote reservation. Optional.
 *   - notes: Additional text associated with the reservation. Optional.
 *
 * @see ding_provider_example_reservation_details()
 */
function ding_provider_example_reservation_list($account) {
  $creds = ding_library_user_get_credentials($account);
  if ($creds == DING_PROVIDER_AUTH_REQUIRED) {
    return $creds;
  }
  // As we don't really have a backend, we just fake it.
  return array(
    // The keys should correspond with the items ids.
    '123' => array(
      // Id of the loan/reservation. Used as identifier. No assumptions are
      // made about it, however it should be a sting value suitable for values
      // in forms and the like.
      'id' => '123',
      // Materials number, optional. Is shown to the user.
      'materials_number' => 43125871,
      // Now, we could return a 'ting_object' here, but for efficiencys sake,
      // we note the id here, and load it in 'details'. For responses without
      // a ting reference, see the next entry.
      'provider_id' => 24907511, // Terry Pratchett, Monstrous regiment
      // Status of the reservation, 'active' or 'fetchable'
      'status' => 'active',
      // Description of pickup place.
      'pickup_branch' => 'Somewhere',
      // Creation date of reservation. Format: YYYY-MM-DD.
      'create_date' => strftime('%Y-%m-%d', strtotime('-7 days')),
      // Reservation valid from. Format: YYYY-MM-DD.
      'valid_from' => strftime('%Y-%m-%d', strtotime('-7 days')),
      // Reservation valid to. Format: YYYY-MM-DD.
      'valid_to' => strftime('%Y-%m-%d', strtotime('+30 days')),

      // For 'active' reservations:
      // Queue number. optional.
      'queue_number' => 3,
      //Notes - some additional information
      'notes' => "Rincewind was here.", 
  
      // For 'fetchable' reservations, see the next entry.
    ),
    'euaeu' => array(
      'id' => 'euaeu', // string id...
      // Title of the item, optional.
      'title' => 'Hidden Empire',
      // Authors of the item, optional. Not shown if display_title is used.
      'authors' => 'Kevin J. Anderson',
      // Alternative title string, optional. Used if title is empty. If
      // neither 'title' nor 'display_title' is specified, an 'Title not
      // available' message is displayed.
      'display_title' => 'Kevin J. Anderson: Hidden Empire',
      // Status of the reservation, 'active' or 'fetchable'
      'status' => 'fetchable',
      // Description of pickup place.
      'pickup_branch' => 'Somewhere else',
      // Creation date of reservation. Format: YYYY-MM-DD.
      'create_date' => strftime('%Y-%m-%d', strtotime('-7 days')),
      // Reservation valid from. Format: YYYY-MM-DD.
      'valid_from' => strftime('%Y-%m-%d', strtotime('-7 days')),
      // Reservation valid to. Format: YYYY-MM-DD.
      'valid_to' => strftime('%Y-%m-%d', strtotime('+30 days')),

      // For 'fetchable' reservations:
      // Pickup number (?)
      'pickup_number' => 7,
      // Expiry of pickup.
      'pickup_expire_date' => strftime('%Y-%m-%d', strtotime('+4 days')),
    ),
  );
}

/**
 * Add details to a reservation for detailed display.
 *
 * The 'list' callback should only return minimal data, and leave out
 * data that would requiring querying additional services, for
 * speed. 'details' will be called when more data is needed, and should
 * populate extra data for display.
 *
 * Should implement internal per-request caching as it might be called
 * multiple times for the same reservation, preferably using ctools_static.
 *
 * @param array $reservations
 *   The reservation to add details to.
 * @return
 *   The reservation with details added.
 */
function ding_provider_example_reservation_details($reservation) {
  $cache = &ctools_static(__FUNCTION__, array());
  if (!isset($cache[$reservation['id']])) {
    $reservation['extra'] = 'Tadaaa!';
    if ($reservation['provider_id'] and $object = ting_get_object_by_local_id($reservation['provider_id'])) {
      $reservation['ting_object'] = $object;
    }
    $cache[$reservation['id']] = $reservation;
  }
  return $cache[$reservation['id']];
}


/**
 * Create a reservation.
 *
 * Should use any available user defaults for interest period or
 * pickup branch.
 *
 * @param object $account
 *   The user to reserve for.
 * @param array $provider_id
 *   Id to reserve.
 * @return array
 *   with the following keys:
 *   - success:
 *     One of the following constants:
 *     - DING_PROVIDER_AUTH_REQUIRED: Authentication required.
 *     - DING_RESERVATION_SUCCESS: Successfully reserved.
 *     - DING_RESERVATION_ERROR_NO_DEFAULT_BRANCH: User haven't set a default
 *       branch.
 *     - DING_RESERVATION_ERROR_RESERVATION_EXISTS: Already reserved.
 *     - DING_RESERVATION_ERROR_NOT_AVAILABLE: Not available for reservation.
 *     - DING_RESERVATION_ERROR_NOT_ALLOWED: Reservation is not allowed for
 *       user.
 *     - DING_RESERVATION_ERROR: General error (use sparingly).
 *     Any other value is treated like DING_RESERVATION_ERROR.
 *   - branch: (optional) In case of DING_RESERVATION_SUCCESS, the branch
 *     the material is reserved to.
 */
function ding_provider_example_reservation_create($account, $provider_id) {
  return FALSE;
}

/**
 * Check if a reservation exists.
 *
 * @param object $account
 *   The user to check for.
 * @param array $provider_id
 *   Id to reserve.
 * @return boolean
 *   TRUE or FALSE.
 */
function ding_provider_example_reservation_exists($account, $provider_id) {
  return FALSE;
}

/**
 * Return options form fragment for updating/creating reservations.
 *
 * @param object $account
 *   User.
 * @param boolean $create
 *   Whether it's for creating or updating reservations.
 * @return array
 *   Form fragment.
 */
function ding_provider_example_reservation_options($account, $create = TRUE) {

}

/**
 * Update reservations.
 *
 * @param object $account
 *   The user.
 * @param array $reservation_ids
 *   The reservations to update.
 * @param array $options
 *   The options gathered through reservation_options.
 * @return array
 *   Reservation_id => TRUE for success, FALSE or an error message string
 *   for failures.
 */
function ding_provider_example_reservation_update($account, $reservation_ids, $options = array()) {

}

/**
 * Delete reservations.
 *
 * @param object $account
 *   The user to delete for.
 * @param array $reservation_ids
 *   The reservations to delete.
 * @return array
 *   Reservation_id => success.
 */
function ding_provider_example_reservation_delete($account, $reservation_ids) {

}
