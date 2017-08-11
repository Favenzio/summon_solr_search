<?php
/**
 * @file
 * Default theme implementation for search results page.
 */
?>

<form action="/" method="post" id="onesearch-inline-form" accept-charset="UTF-8">
  <input placeholder="<?php print $search_input; ?>" type="text" name="onesearch_block_input"
    value="" size="25" maxlength="128" class="form-text required" />
  <input type="submit" class="edit-onesearch-block-submit" name="op" value="Search" class="form-submit" />
  <input type="hidden" name="onesearch_block_type" value="website" />
  <input type="hidden" name="form_id" value="onesearch_form" />
</form>

<div class="summon-search-menu" id="ss-menu-desktop">
  <ul>
    <li><a class="summon-search-databases ss-active" href="#">Databases</a></li>
    <li><a class="summon-search-website" href="#">Website & Research Guides</a></li>
  </ul>
</div>

<div class="summon-search-menu" id="ss-menu-mobile">
  <ul>
    <ul>
      <li class="ss-extended"><a class="summon-search-databases" href="#">Databases</a></li>
      <li class="ss-extended"><a class="summon-search-website" href="#">Website & Research Guides</a></li>
    </ul>
  </ul>
</div>

<div id="1" class="summon-search-container">
  <div id="ajax-loader-1" class="ajax-loader">
    <p><img src="<?php print '/' . drupal_get_path('module', 'summon_solr_search')
      . '/images/ajax-loader.gif' ?>"></p>
  </div>
</div>
