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
  $data = json_decode($json);
  
  echo "<pre>";
  print_r($data);
  echo "</pre>";
}

