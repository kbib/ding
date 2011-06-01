<?php
// $Id$
/**
 * @file ding-library-user-list-item.tpl.php
 *
 * Template to render an item in loan/reservations lists.
 *
 * Available variables:
 * - $materials_number: Materials number (optional).
 * - $title: Item title, possibly a link.
 * - $title_plain: Title without a link.
 * - $image: Image (optional).
 * - $authors: Authors (optional).
 * - $type: Item type (optional).
 * - $url: Url of item.
 * - $remote: Remote loan/reservations indicator (string).
 */
?>
<!-- ding-library-user-list-item.tpl -->
<span class="title"><?php print $title; ?></span>
<?php if ($authors or $type) { ?>
  <span class="creator">
  <?php if ($authors) { ?>
    <span class="byline"><?php print t('by'); ?></span>
    <?php print $authors; ?>
  <?php } ?>
  <?php if ($type) { ?>
    <span class="date">(<?php print $type; ?>)</span>
  <?php } ?>
</span>
<?php } ?>
<?php if ($remote) { ?>
  <span class="remote">(<?php print $remote; ?>)</span>
<?php } ?>
<?php if ($materials_number) { ?>
    (<span class="material-number"><?php print t('Mat. no @num', array('@num' => $materials_number)); ?></span>)
<?php } ?>
<!-- /ding-library-user-list-item.tpl -->
