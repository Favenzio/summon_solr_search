<?php
/**
 * @file
 * Default theme implementation for search results page.
 */
?>

<p>Your search for <strong><?php print $search_input; ?></strong>
returned the following results:</p>

<div id="summon-search-menu">
  <ul>
    <li><a id="summon-search-all" href="#" class="ss-active">All</a></li>  
    <li><a id="summon-search-articles" href="#">Articles</a></li>
    <li><a id="summon-search-books" href="#">Books</a></li>
    <li><a id="summon-search-journals" href="#">Journals</a></li>
    <li><a id="summon-search-databases" href="#">Databases</a></li>
    <li><a id="summon-search-website" href="#">Website</a></li>    
  </ul>
</div>

<div id="1" class="summon-search-container">
  <div id="ajax-loader-1" class="ajax-loader">
    <p><img src="<?php print '/' . drupal_get_path('module', 'summon_solr_search') 
      . '/images/ajax-loader.gif' ?>"></p>
  </div>
</div>