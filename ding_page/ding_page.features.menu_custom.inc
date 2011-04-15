<?php

/**
 * Implementation of hook_menu_default_menu_custom().
 */
function ding_page_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-organisation
  $menus['menu-organisation'] = array(
    'menu_name' => 'menu-organisation',
    'title' => 'Organisation',
    'description' => 'Default menu for pages which are not associated with a specific library',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Default menu for pages which are not associated with a specific library');
  t('Organisation');


  return $menus;
}
