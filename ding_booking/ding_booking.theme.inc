<?php

/**
 * Theming of booking detailed list form.
 */
function theme_ding_booking_list_form($form) {
  $date_format = variable_get('date_format_date_short', 'Y-m-d');
  $output = '';
  module_load_include('client.inc', 'ting');
  $states = array(
    'deficit' => t('Incompletely returned bookings'),
    'waiting' => t('Future bookings'),
    'ready' => t('Bookings ready for pickup'),
    'active' => t('Active bookings'),
    'closed' => t('Closed bookings'),
  );

  foreach ($states as $status => $title) {
    if (!isset($form['bookings']['#grouped'][$status])) {
      continue;
    }
    $bookings = $form['bookings']['#grouped'][$status];
    $header = array(
      t('Select'),
      '',
      t('Title'),
      t('From'),
      t('To'),
      t('Count'),
      t('Pickup branch'),
    );

    $colgroups = array(
      array(
        array(
          'class' => 'col-selection',
        ),
      ),
      array(
        array(
          'class' => 'col-image',
        ),
      ),
      array(
        array(
          'class' => 'col-title',
        ),
      ),
      array(
        array(
          'class' => 'col-start-date',
        ),
      ),
      array(
        array(
          'class' => 'col-end-date',
        ),
      ),
      array(
        array(
          'class' => 'col-pickup-branch',
        ),
      ),
    );

    $rows = array();

    foreach ($bookings as $item) {
      $cells = array();
      $cells['checkbox'] = array(
        'class' => 'checkbox',
        'data' => drupal_render($form['selected'][$item['id']]),
      );

      $cells['image'] = array(
        'class' => 'image',
        'data' => theme('ding_library_user_list_item_image', 'booking', $item, '80_x'),
      );

      $cells['title'] = array(
        'class' => 'title',
        'data' => theme('ding_library_user_list_item', 'booking', $item),
      );

      $cells['start_date'] = array(
        'class' => 'start_date',
        'data' => ding_library_user_format_date($item['start_date'], $date_format),
      );

      $cells['end_date'] = array(
        'class' => 'end_date',
        'data' => ding_library_user_format_date($item['end_date'], $date_format),
      );

      $cells['count'] = array(
        'class' => 'count',
        'data' => $item['count'],
      );

      $cells['pickup_branch'] = array(
        'class' => 'pickup_branch',
        'data' => $item['pickup_branch'] ? $item['pickup_branch'] : t('Unknown'),
      );

      $rows[] = $cells;
    }

    $output .= theme('table', $header, $rows, array('id' => 'booking-' . $status, 'colgroups' => $colgroups), $title);
  }

  if (empty($output)) {
    return t('No bookings found.');
  }

  $output .= '<div class="update-controls clear-block">';
  $output .= drupal_render($form['options']);
  $output .= '</div>';

  // fisk
  $output .= '<div class="update-controls-button clear-block">';
  $output .= drupal_render($form['buttons']);
  $output .= '</div>';

  $output .= drupal_render($form);

  return $output;
}
