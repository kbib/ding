<?php

/**
 * @file ding_panels_content_library_location.tpl.php
 * Library location, map, etc.
 */

/**
 * The address, etc. are marked up to conform to the hCard microformat.
 * http://microformats.org/wiki/hcard for more information.
 */
?>
<div class="library-info">
  <div class="library-info-inner">

    <div class="vcard">
      <span class="fn org"><?php print $node->title; ?></span>
      <div class="adr">
        <div class="street-address"><?php print $node->location['street']; ?></div>
        <span class="postal-code"><?php print $node->location['postal_code']; ?></span>
        <span class="locality"><?php print $node->location['city']; ?></span>
        <div class="country-name"><?php print $node->location['country_name']; ?></div>
      </div>
      <?php if($node->location['phone']){ ?>
      <div class="tel">
        <span class="type"><?php print t('Phone'); ?></span> <?php print $node->location['phone']; ?>
      </div>
      <?php } ?>
      <?php if($node->location['fax']){ ?>
      <div class="tel">
        <span class="type"><?php print t('Fax'); ?></span> <?php print $node->location['fax']; ?>
      </div>
      <?php } ?>
      <?php if($node->field_email['0']['email']){ ?>
      <div class="email">
            <span class="type"><?php print t('E-mail'); ?>:</span> <span><?php print spamspan(l($node->field_email['0']['email'], 'mailto:' . $node->field_email['0']['email'])); ?></span>
      </div>
      <?php } ?>

    </div>

    <?php print $library_links; ?>
  </div>

  <div class="library-map">
    <div class="library-map-inner">
    <?php print $library_map; ?>
    </div>
  </div>

</div>
