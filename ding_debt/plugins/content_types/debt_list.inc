<?php

/**
 * @file debt_list.inc
 * Provides debt related ctools content types.
 */

$plugin = array(
  'title' => t('Library patron profile'),
);

/**
 * Callback function to supply a list of content types.
 */
function ding_debt_debt_list_content_type_content_types() {
  $types = array(
    'ding_debt_notice' => array(
      'title' => t('User debts notice'),
      'description' => t('Provides a short noice to the user about his library debts.'),
    ),
    'ding_debt_list_form' => array(
      'title' => t('User debts details'),
      'description' => t('Provides the full list of user debts, with an option to pay them.'),
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
function ding_debt_debt_list_content_type_render($subtype, $conf, $panel_args, $context) {
  $account = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block = new stdClass();

  if (ding_provider_invoke('user', 'logged in', $account)) {
    switch ($subtype) {
      case 'ding_debt_notice':
        $debts = ding_provider_invoke('debt', 'list', $account);
        $debt_with_note = NULL;
        foreach ($debts as $debt) {
          if ($debt['display_title']) {
            $debt_with_note = $debt;
            break;
          }
        }
        $total_debt = _ding_debt_list_get_total($debts);
        if ($total_debt <= 0) {
          break;
        }
        $block->content = t('You owe @amount kr.', array('@amount' => $total_debt,));
        if ($debt_with_note) {
          $block->content .= ' ' . t('for “%note”', array(
            '%note' => trim($debt_with_note['display_title']),
          ));
        }

        if (count($debts) > 1) {
          if ($debt_with_note) {
            $block->content .= ' ' . t('and !debt_others_link', array(
                                 '!debt_others_link' => l(format_plural(count($debts) - 1, 'one other', '@count others'),
                                                      'user/' . $account->uid . '/debts'),
            ));
          }
          else {
            $block->content .= ' ' . t('for !debts_link', array(
                                 '!debts_link' => l(format_plural(count($debts) - 1, 'one debt', '@count debts'),
                                'user/' . $account->uid . '/debts'),
            ));
          }
        }

        if(module_exists('ding_dibs')){
          $block->content .= t('. !debt_pay_link.', array(
              '!debt_pay_link' => l(t('Pay now'), 'user/' . $account->uid . '/debts'),
            ));
        }

        break;

      case 'ding_debt_list_form':
        $data = ding_provider_invoke('debt', 'list', $account);
        $block->content = drupal_get_form('ding_debt_list_form', $account, $data);
        break;
    }
  }

  return $block;
}

/**
 * Return the pane titles.
 */
function ding_debt_debt_list_content_type_admin_title($subtype, $conf, $context) {
  switch($subtype) {
    case 'ding_debt_notice':
      return t('“@s”: Debts notice', array('@s' => $context->identifier));
    case 'ding_debt_list_form':
      return t('“@s”: Debts list', array('@s' => $context->identifier));
  }
}

/**
 * Returns an edit form for the content type.
 *
 * We're not actually doing anything here, but we need to have this hook
 * for this content type to work.
 */
function ding_debt_debt_list_content_type_edit_form(&$form, &$form_state) {
  return $form;
}

