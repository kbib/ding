<?php
// $Id$

/**
 * @file
 * Template to render KultuNaut activities XML feed.
 */

// Fallback encoding is UTF-8.
if (empty($feed['encoding'])) { $feed['encoding'] = 'utf-8'; }

// Print the XML declaration in case PHP has short_open_tag enabled.
print '<?xml version="1.0" encoding="'. $feed['encoding'] .'"?>'."\n";
?>
<activities>
<?php print $feed['activities']; ?>
</activities>

