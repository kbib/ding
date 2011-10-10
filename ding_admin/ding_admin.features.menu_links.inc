<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function ding_admin_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: admin:admin/content
  $menu_links['admin:admin/content'] = array(
    'menu_name' => 'admin',
    'link_path' => 'admin/content',
    'router_path' => 'admin/content',
    'link_title' => 'Content management',
    'options' => array(
      'attributes' => array(
        'title' => 'Manage your site\'s content.',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '1',
    'expanded' => '0',
    'weight' => '-10',
    'parent_path' => 'admin',
  );
  // Exported menu link: admin:node/add
  $menu_links['admin:node/add'] = array(
    'menu_name' => 'admin',
    'link_path' => 'node/add',
    'router_path' => 'node/add',
    'link_title' => 'Create content',
    'options' => array(
      'attributes' => array(
        'title' => 'Create content for your site.',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-50',
    'parent_path' => 'admin/content',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Content management');
  t('Create content');


  return $menu_links;
}
