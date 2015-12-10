<?php
/**
 * A Collection of helper functions to query SOLR
 */

/**
 *  Adds a maximum of 2 results with a relevance score higher than 1.5
 *  to the blended_results array (passed by reference)
 */
function _solr_title_search($solr_host, $solr_port, $solr_core, $keywords, &$blended_results) {
  $count = 0;  
  $filter = 'http://' . $solr_host . ':' . $solr_port . '/solr/' . $solr_core
    . '/select?q=tm_title%3A' . $keywords 
    . '&fl=score%2Css_url%2Css_type%2Ctm_title%2Ccontent&wt=json&indent=true';
  
  $title_search_results = json_decode(file_get_contents($filter));
  
  foreach($title_search_results->response->docs as $result) {
    if($result->score > 1.5 && $count < 2) {
      $blended_results[] = array (
        'type' => $result->ss_type,
        'title' => $result->tm_title[0],
        'url' => $result->ss_url,
        'content' => $result->content, 
      );
      
      $count++;
    }
  }
}

/**
 *  Adds a maximum of 2 results with a relevance score higher than 0.7
 *  to the blended_results array (passed by reference)
 */
function _solr_generic_search($solr_host, $solr_port, $solr_core, $keywords, &$blended_results) {
  $count = 0;  
  $filter = 'http://' . $solr_host . ':' . $solr_port . '/solr/' . $solr_core
    . '/select?q=' . $keywords 
    . '&fl=score%2Css_url%2Css_type%2Ctm_title%2Ccontent&wt=json&indent=true';
  
  $title_search_results = json_decode(file_get_contents($filter));
  
  foreach($title_search_results->response->docs as $result) {
    if($result->score > 0.7 && $count < 2) {
      $blended_results[] = array (
        'type' => $result->ss_type,
        'title' => $result->tm_title[0],
        'url' => $result->ss_url,
        'content' => $result->content, 
      );
      
      $count++;
    }
  }
}