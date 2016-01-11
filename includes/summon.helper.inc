<?php
/**
 * A Collection of helper functions to query Summon
 */

/**
 *  Adds a maximum of 2 results with a relevance score higher than 1.5
 *  to the blended_results array (passed by reference)
 */
function _summon_generic_search($auth_info, $keywords, &$blended_results) {
  
  global $base_url;
  
  $summon_request = $base_url . '/cgi-bin/summon_connector.py?';
  $summon_request .= 'id=' . $auth_info['id'];
  $summon_request .= '&key=' . $auth_info['key'];
  $summon_request .= '&st=all&kw=' . $keywords;
  
  $json = file_get_contents($summon_request);
  $results = json_decode($json);
  
  /*print '<pre>';
  print_r($results);
  print '</pre>';*/
  
  foreach ($results->documents as $result) {
    $result_item = _create_result_item($result);    
    $blended_results[] = $result_item;
  }
}

function _create_result_item($result) {
  $result_array = array();
  $author_field = 'by ';
  $author_count = 0;
  
  $result_array['title'] = $result->Title[0];
  $result_array['url'] = $result->link;
  $result_array['type'] = $result->ContentType[0];
  $result_array['score'] = $result->Score[0];
  
  if (isset($result->Abstract[0]))
  {
    $result_array['content'] = $result->Abstract[0];
  }
  
  if (isset($result->Author) && count($result->Author) > 0) {
    foreach ($result->Author as $author) {
      $author_count == 0 ? $author_field .= $author : $author_field .= ' and ' . $author;
      $author_count++;
    }
    $result_array['author'] = $author_field;
  }
  
  if (isset($result->hasFullText) && $result->hasFullText == 1) {
    $result_array['full_text'] = 'true';
  } else {
    $result_array['full_text'] = 'false';
  }
  
  if (isset($result->SubjectTermsDisplay[0]))
  {
    $result_array['subject_terms'] = $result->SubjectTermsDisplay[0];
  }
  
  return $result_array;
}
