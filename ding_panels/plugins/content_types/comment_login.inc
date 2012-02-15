<?php

/**
 * @file comment_login.inc
 * Provides several library portal page related panels content types.
 */


/**
 * Callback function to supply a list of content types.
 *
 * This is only required to make CTools know that we're providing 
 * multiple types here (since we have no single => TRUE in the array).
 */
function ding_panels_comment_login_ctools_content_types() {
  return array(
    'title' => t('Node'),
  );
}

/**
 * Callback function to supply a list of content types.
 */
function ding_panels_comment_login_content_type_content_types() {
  return array(
    'comment_login' => array(
      'title' => t('Comment login'),
      'description' => t('If there is no comments on a node, this pane will say so, and if applicable, tell the user to login to be able to post comments.'),
      'category' => t('Node'),
      'required context' => new ctools_context_required(t('Node'), 'node'),
    ),
  );
}

/**
 * Return the pane contents.
 */
function ding_panels_comment_login_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $node = isset($context->data) ? $context->data : NULL;

  if ($node instanceOf stdClass && $node->comment_count < 1) {
    $message = t('No comments for this post');

    $forbidden = theme_comment_post_forbidden($node);

    if ($forbidden) {
      $message .= ' – ' . $forbidden;
    }
  }

  if (isset($message)) {
    $block->content = '<p class="comment-login">' . $message . '.</p>';
  }

  return $block;
}

/**
 * Return the pane titles.
 */
function ding_panels_comment_login_content_type_admin_title($subtype, $conf, $context) {
  return t('“@s”:  comment login', array('@s' => $context->identifier));
}

/**
 * Returns an edit form for the content type.
 *
 * We're not actually doing anything here, but we need to have this hook 
 * for this content type to work.
 */
function ding_panels_comment_login_content_type_edit_form(&$form, &$form_state) {
  return $form;
}


