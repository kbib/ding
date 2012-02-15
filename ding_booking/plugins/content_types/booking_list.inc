<?php

/**
 * @file booking_list.inc
 * Provides booking related ctools content types.
 */

$plugin = array(
  'title' => t('Library patron profile'),
);

/**
 * Callback function to supply a list of content types.
 */
function ding_booking_booking_list_content_type_content_types() {
  $types = array(
    'ding_booking_list' => array(
      'title' => t('Booking list'),
      'description' => t('A simple list of the user’s bookings.'),
    ),
    'ding_booking_list_form' => array(
      'title' => t('Booking details'),
      'description' => t('A detailed list of the user’s bookings.'),
    ),
  );

  // Append our default settings to every type.
  foreach ($types as $name => $type) {
    $types[$name] += array(
      'category' => t('Library patron profile'),
      'required context' => new ctools_context_required(t('user'), 'user'),
    );
  }

  return $types;
}

/**
 * Return the pane contents.
 */
function ding_booking_booking_list_content_type_render($subtype, $conf, $panel_args, $context) {
  $account = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block = new stdClass();

  if (ding_provider_invoke('user', 'logged in', $account)) {
    switch ($subtype) {

      case 'ding_booking_list':
        $list = ding_provider_invoke('booking', 'list', $account);
        if ($list != DING_PROVIDER_AUTH_REQUIRED) {
          $block->title = t('Your bookings');
          $block->content = theme('ding_library_user_record_list', 'booking', $list);
        }
        break;

      case 'ding_booking_list_form':
        module_load_include('forms.inc', 'ding_library_user');
        $list = ding_provider_invoke('booking', 'list', $account);
        $block->title = t('Your bookings');
        $block->content = drupal_get_form('ding_booking_list_form', $account, $list);
        break;
    }
  }

  return $block;
}

/**
 * Return the pane titles.
 */
function ding_booking_booking_list_content_type_admin_title($subtype, $conf, $context) {
  switch($subtype) {
    case 'ding_booking_list':
      return t('“@s”: Booking list', array('@s' => $context->identifier));
    case 'ding_booking_list_form':
      return t('“@s”: Booking details', array('@s' => $context->identifier));
  }
}

/**
 * Returns an edit form for the content type.
 *
 * We're not actually doing anything here, but we need to have this hook
 * for this content type to work.
 */
function ding_booking_booking_list_content_type_edit_form(&$form, &$form_state) {
  return $form;
}

