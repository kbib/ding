<?php
// $Id$

/**
 * @file ding_box.inc
 * Definition of the 'ding_box' pane style.
 */

/**
 * Implementation of hook_panels_style_info().
 */
function ding_panels_ding_box_panels_styles() {
  return array(
    'ding_box' => array(
      'title' => t('Ding box'),
      'description' => t('Presents panes with a neat box around it as per the Ding design.'),
      'render pane' => 'ding_panels_ding_box_style_render_pane',
    ),
  );
}

/**
 * Render callback for a single pane.
 */
function theme_ding_panels_ding_box_style_render_pane($content, $pane, $display) {
  return '<div class="ding-box-pane">' . theme('panels_pane', $content, $pane, $display) . '</div>';
}

