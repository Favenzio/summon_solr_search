<?php
/**
 * @file
 * Default theme implementation for search results page.
 */
?>

<p>Your search for <strong><?php print $search_input; ?></strong>
returned the following results:</p>

<div class="summon-search-menu" id="ss-menu-desktop">
  <ul>
    <li><a class="summon-search-all ss-active" href="#">All</a></li>  
    <li><a class="summon-search-articles" href="#">Articles</a></li>
    <li><a class="summon-search-books" href="#">Books</a></li>
    <li><a class="summon-search-journals" href="#">Journals</a></li>
    <li><a class="summon-search-databases" href="#">Databases</a></li>
    <li><a class="summon-search-website" href="#">Website & Research Guides</a></li>    
  </ul>
</div>

<div class="summon-search-menu" id="ss-menu-mobile">
  <ul>
    <li><a class="summon-search-all ss-active" href="#">All</a></li>
    <li><a class="summon-search-articles" href="#">Articles</a></li>
    <li><a class="summon-search-books" href="#">Books</a></li>
    <li><a class="summon-search-more" href="#">More...</a></li>
    <ul id="ss-extended-menu" class="ss-invisible">
      <li><a class="summon-search-journals" href="#">Journals</a></li>
      <li><a class="summon-search-databases" href="#">Databases</a></li>
      <li><a class="summon-search-website" href="#">Website & Research Guides</a></li>
    </ul>
  </ul>
</div>

<div id="1" class="summon-search-container">
  <div id="ajax-loader-1" class="ajax-loader">
    <p><img src="<?php print '/' . drupal_get_path('module', 'summon_solr_search') 
      . '/images/ajax-loader.gif' ?>"></p>
  </div>
</div>
