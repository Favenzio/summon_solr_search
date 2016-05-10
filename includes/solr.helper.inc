<?php
/**
 * A Collection of helper functions to query SOLR
 */

/**
 *  Adds a maximum of @n_results with a relevance score higher than @score
 *  to the blended_results array (passed by reference)
 */
function _solr_title_search($score, $n_results, $solr_host, $solr_port, 
$solr_core, $keywords, &$blended_results) {
  $count = 0;  
  $filter = 'http://' . $solr_host . ':' . $solr_port . '/solr/' . $solr_core
    . '/select?q=tm_title%3A' . $keywords 
    . '&fl=score%2Css_url%2Css_type%2Ctm_title%2Ccontent&wt=json&indent=true';
  
  $title_search_results = json_decode(file_get_contents($filter));
  
  foreach($title_search_results->response->docs as $result) {
    if($result->score > $score && $count < $n_results) {
      $blended_results[] = array (
        'type' => $result->ss_type,
        'title' => $result->tm_title[0],
        'url' => $result->ss_url,
        'content' => $result->content,
        'score' => $result->score,
      );
      
      $count++;
    }
  }
}

/**
 *  Adds up to 50 results with a relevance score higher than @score
 *  to the blended_results array (passed by reference)
 */
function _solr_generic_search($score, $solr_host, $solr_port, $solr_core, $keywords, &$blended_results) { 
  $filter = 'http://' . $solr_host . ':' . $solr_port . '/solr/' . $solr_core
    . '/select?q=' . $keywords 
    . '&rows=50&fl=score%2Css_url%2Css_type%2Ctm_title%2Ccontent&wt=json&indent=true';
  
  $title_search_results = json_decode(file_get_contents($filter));
  
  foreach($title_search_results->response->docs as $result) {
    if($result->score > $score) {
      $blended_results[] = array (
        'type' => $result->ss_type,
        'title' => $result->tm_title[0],
        'url' => $result->ss_url,
        'content' => $result->content,
        'score' => $result->score,
      );
    }
  }
}

/**
 *  Adds a maximum of @n_results results of @type
 *  to the blended_results array (passed by reference)
 */
function _solr_type_search($type, $n_results, $solr_host, $solr_port, 
$solr_core, $keywords, &$blended_results) {
  $count = 0;  
  $filter = 'http://' . $solr_host . ':' . $solr_port . '/solr/' . $solr_core
    .'/select?q=tm_title%3A' . $keywords . '+AND+ss_type%3A' . $type 
    . '&fl=score%2Css_url%2Css_type%2Ctm_title%2Ccontent&wt=json&indent=true';
  
  $title_search_results = json_decode(file_get_contents($filter));
  
  foreach($title_search_results->response->docs as $result) {
    if($count < $n_results) {
      $blended_results[] = array (
        'type' => $result->ss_type,
        'title' => $result->tm_title[0],
        'url' => $result->ss_url,
        'content' => $result->content,
        'score' => $result->score,
      );
      
      $count++;
    }
  }
  
  if($count < $n_results) {
    $second_filter = 'http://' . $solr_host . ':' . $solr_port . '/solr/' . $solr_core
      .'/select?q=' . $keywords . '+AND+ss_type%3A' . $type 
      . '&fl=score%2Css_url%2Css_type%2Ctm_title%2Ccontent&wt=json&indent=true';
    
    $generic_search_results = json_decode(file_get_contents($second_filter));
    
    foreach($generic_search_results->response->docs as $result) {
      if($count < $n_results) {
        $blended_results[] = array (
          'type' => $result->ss_type,
          'title' => $result->tm_title[0],
          'url' => $result->ss_url,
          'content' => $result->content,
          'score' => $result->score,
        );
      
        $count++;
      }
    }
  }
}

/**
 *  Adds a maximum of @n_results results without @type
 *  to the blended_results array (passed by reference)
 */
function _solr_remove_type_search($type, $n_results, $solr_host, $solr_port, 
$solr_core, $keywords, &$blended_results) {
  
  _solr_title_search(1.5, 2, $solr_host, $solr_port, $solr_core, $keywords, $blended_results); 
  
  if(count($blended_results) < $n_results) {
    _solr_generic_search(0, $solr_host, $solr_port, $solr_core, $keywords, $blended_results);
  }
  
  foreach ($blended_results as $key => $result) {
    if($result['type'] == $type) {
      unset($blended_results[$key]);
    }
  }
}