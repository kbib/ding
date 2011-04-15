<?php


/**
 * @file
 * Ding booking plugin.
 */

$plugin = array(
  'description' => t('Example booking plugin'),
  'version' => 1,
);

/**
 * Get the users current bookings.
 *
 * Returns an array of id => details where each details is an array
 * consisting of:
 * - id: Id of the booking, same as the key of the outer arrayp
 * - display_title: Description shown to the user.
 * - status: Status of the booking, one of the following strings:
 *   - waiting: Not yet active.
 *   - active: Materials are being put aside as they're returned.
 *   - ready: Ready for pickup.
 *   - deficit: Incompletely returned.
 *   - closed: Closed and done.
 * - start_date: When the booking starts. Format YYYY-MM-DD.
 * - end_date: When the booking ends. Format YYYY-MM-DD.
 * - count: The number of materials wanted.
 * - fetched_count: How many has been fetched (Optional).
 * - loaned_count: How many has been loaned (Optional).
 * - returned_count: How many has been returned (Optional).
 * - pickup_branch: The pickup branch of the booking.
 *
 * @param $account
 *   User object of the user.
 * @return array
 *   Bookings data.
 */
function ding_provider_example_booking_list($account) {
}

/**
 * Booking options form.
 *
 * Called by ding_booking for presenting booking options. Should
 * return part of a form. You should use #element_validate to ensure
 * that the options passed to the 'booking create' function is valid.
 *
 * @param $account
 *   The user booking.
 * @return array
 *   A form fragment.
 */
function ding_provider_example_booking_options($account) {
  $form['start_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('From'),
    '#required' => TRUE,
    '#date_format' => DATE_FORMAT_DATE,
    '#date_year_range' => '-0:+2',
    '#element_validate' => array('ding_base_element_validate_future_date'),
  );

  $form['end_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('To'),
    '#required' => TRUE,
    '#date_format' => DATE_FORMAT_DATE,
    '#date_year_range' => '-0:+2',
    '#element_validate' => array('ding_base_element_validate_future_date'),
  );

  $form['count'] = array(
    '#type' => 'textfield',
    '#title' => t('Number to book'),
    '#required' => TRUE,
    '#default_value' => '1',
    '#element_validate' => array('ding_base_element_validate_positive_integer'),
  );

  $form['pickup_branch'] = array(
    '#type' => 'select',
    '#title' => t('Pickup branch'),
    '#required' => TRUE,
    '#options' => array(),
  );

  return $form;
}

/**
 * Create a booking.
 *
 * @param object $account
 *   The user to book for.
 * @param array $provider_id
 *   Id to book.
 * @param $optional
 *   The form values from the 'booking options' form above.
 * @return array
 *   with the following keys:
 *   - success:
 *     One of the following constants:
 *     - DING_PROVIDER_AUTH_REQUIRED: Authentication required.
 *     - DING_BOOKING_SUCCESS: Successfully reserved.
 *     - DING_BOOKING_ERROR: General error (use sparingly).
 *     Any other value is treated like DING_BOOKING_ERROR.
 *   - message: (optional) In case of DING_BOOKING_ERROR, an optional error
 *     message which is displayed to the user instead of the standard error
 *     message.
 */
function ding_provider_example_booking_create($account, $provider_id, $optional = array()) {
}

/**
 * Delete bookings.
 *
 * @param object $account
 *   The user to book for.
 * @param array $booking_ids
 *   The bookings to delete.
 * @return array
 *   Array of booking_id => (bool) success.
 */
function openruth_booking_delete($account, $booking_ids) {
  $result = array();
  foreach ($bookings_ids as $bookings_id) {
    $result[$bookings_id] = openruth_client_invoke('cancel_booking', $bookings_id);
  }

  // Flush cached user status.
  _openruth_user_status();

  return $result;
}
