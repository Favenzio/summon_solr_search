<?php
/**
 * @file
 * Default theme implementation for search results page.
 */
?>

<p>Your search for <strong><?php print $search_input; ?></strong>
returned the following results:</p>

<div id="ajax-loader-1" class="ajax-loader">
  <img src="<?php print '/' . drupal_get_path('module', 'summon_solr_search') . '/images/ajax-loader.gif' ?>">
</div>