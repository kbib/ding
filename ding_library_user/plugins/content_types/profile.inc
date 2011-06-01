<?php
// $Id$

/**
 * @file relevant.inc
 * Provides several library portal page related panels content types.
 */

/**
 * Callback function to supply a list of content types.
 *
 * This is only required to make CTools know that we're providing
 * multiple types here (since we have no single => TRUE in the array).
 */
function ding_library_user_profile_ctools_content_types() {
  return array(
    'title' => t('Library user profile'),
  );
}

/**
 * Callback function to supply a list of content types.
 */
function ding_library_user_profile_content_type_content_types() {
  $types = array(
    'ding_library_user_library_settings' => array(
      'title' => t('Library settings'),
      'description' => t('The user’s library settings. Allows the user to choose a preferred library and suspend his reservations for a period.'),
    ),
    'ding_library_user_loan_list' => array(
      'title' => t('Loan list'),
      'description' => t('A simple list of the user’s loans.'),
    ),
    'ding_library_user_loan_list_form' => array(
      'title' => t('Loan details'),
      'description' => t('A detailed list of the user’s loans.'),
    ),
  );

  // Append our default settings to every type.
  foreach ($types as $name => $type) {
    $types[$name] += array(
      'category' => t('Library user profile'),
      'required context' => new ctools_context_required(t('user'), 'user'),
    );
  }

  return $types;
}

/**
 * Return the pane contents.
 */
function ding_library_user_profile_content_type_render($subtype, $conf, $panel_args, $context) {
  $account = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block = new stdClass();

  if (ding_provider_invoke('user', 'logged in', $account)) {
    switch ($subtype) {
      case 'ding_library_user_library_settings':
        if (ding_provider_supports('user', 'library settings form')) {
          $block->title = t('Your library');
          $block->content = ding_provider_get_form('user', 'library settings form', $account);
        }
        break;

      case 'ding_library_user_loan_list':
        $list = ding_provider_invoke('loan', 'get loans', $account);
        if ($list != DING_PROVIDER_AUTH_REQUIRED) {
          $block->title = t('Your loans');
          $block->content = theme('ding_library_user_record_list', 'loan', $list);
        }
        break;
      case 'ding_library_user_loan_list_form':
        module_load_include('forms.inc', 'ding_library_user');
        $list = ding_provider_invoke('loan', 'get loans', $account);
        if ($list != DING_PROVIDER_AUTH_REQUIRED) {
          $block->title = t('Your loans');
          $block->content = drupal_get_form('ding_library_user_loan_list_form', $account, $list);
        }
        break;
    }
  }
  elseif ($subtype == 'ding_library_user_library_settings') {
    // TODO: Change the blocks above for those that should always be
    // shown, to tell the user that they need to log in.
    $block->content = t('Error fetching your library user profile.');
  }

  return $block;
}

/**
 * Return the pane titles.
 */
function ding_library_user_profile_content_type_admin_title($subtype, $conf, $context) {
  switch($subtype) {
    case 'ding_library_user_library_settings':
      return t('“@s”: Library settings', array('@s' => $context->identifier));
    case 'ding_library_user_loan_list':
      return t('“@s”: Loan list', array('@s' => $context->identifier));
    case 'ding_library_user_loan_list_form':
      return t('“@s”: Loan details', array('@s' => $context->identifier));
  }
}

/**
 * Returns an edit form for the content type.
 *
 * We're not actually doing anything here, but we need to have this hook
 * for this content type to work.
 */
function ding_library_user_profile_content_type_edit_form(&$form, &$form_state) {
  return $form;
}

