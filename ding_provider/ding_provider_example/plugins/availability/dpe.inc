<?php


/**
 * @file
 * Ding availability plugin.
 */

$plugin = array(
  'description' => t('Example availability plugin'),
  'version' => 1,
);

/**
 * Check availability of some items.
 *
 * @param $provider_ids
 *   Array of ids of items.
 *
 * @return
 *   Array of id => boolean.
 */
function ding_provider_example_availability_items($provider_ids) {
}

/**
 * Get availability details.
 *
 * For each item, return an array with the following keys:
 * - local_id: The id of the item.
 * - show_reservation_button: Boolean indicating whether an reservation
 *   button can be shown.
 * - available: Whether the item is available for reservation.
 * - reservable: Whether the item can be reserved at all.
 * - total_count: Total number of copies in stock. (Optional)
 * - reservable_count: Total number of reservable copies. (Optional)
 * - reserved_count: Number of copies reserved. (Optional)
 * - holdings: Array of strings of the locations of the material.
 *
 * @param $provider_ids
 *   Array of ids of items.
 *
 * @return
 *   Array of id => details array.
 */
function ding_provider_example_availability_details($provider_ids) {
  $avail = array();
  foreach ($provider_ids as $id) {
    $avail[$id] = array(
      'local_id' => $id,
      'show_reservation_button' => TRUE,
      'available' => TRUE,
      'reservable' => TRUE,
      'total_count' => 6,
      'reservable_count' => 4,
      'reserved_count' => 2,
      'holdings' => array(
        'Andeby bibliotek → voksenafdeling',
        'Gåserød bibliotek → voksenafdeling',
      ),
    );
  }
  return $avail;
}
