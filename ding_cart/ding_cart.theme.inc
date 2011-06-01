<?php
// $Id$

/**
 * Rendering of Ding cart item.
 */
function theme_ding_cart_item($item) {
  $output = '<span class="title">' . l($item['title'], 'ting/object/' . $item['ting_id'])  . '</span> ';
  $output .= t('by');
  $output .= ' <span class="author">' . $item['author'] . '</span>';
  $output .= ' <span class="type">' . $item['type'] . '</span>';
  return $output;
}

