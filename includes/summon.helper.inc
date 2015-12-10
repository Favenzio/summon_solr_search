<?php
/**
 * A Collection of helper functions to query Summon
 */

/**
 *  Adds a maximum of 2 results with a relevance score higher than 1.5
 *  to the blended_results array (passed by reference)
 */
function _summon_generic_search($auth_info, $keywords, &$blended_results) {
  $offset = 0;
  $limit = 10;
  $doctype= "xml";
  $host = $auth_info['host'];
  
  // create query string
  $query = "s.q=" . $keywords . "&fvf=ContentType,Book Review,t&l=en&s.pn=$offset&s.ps=$limit&s.ho=true";
  
  // create request headers
  $accept = "application/$doctype";
  $date = gmstrftime("%a, %d %b %Y %H:%M:%S GMT", time());
  $id_string = implode("\n", array($accept, $date, $auth_info['host'], $auth_info['path'], $query, ""));
  $digest = base64_encode(hash_hmac("sha1", utf8_encode($id_string), $auth_info['key'], True));
  $authorization = "Summon " . $auth_info['id'] . ";" . $digest;
  $headers = array("Host:$host", "Accept:$accept", "x-summon-date:$date", "Authorization:$authorization");
  
  $url = 'http://' . $host . $auth_info['path'] . '?' . $query;
  $response = _summon_url_request($url, $timeout=10, $headers=$headers);
  
  print_r($response);
}

function _summon_url_request($url, $timeout=10, $headers=array()) {
  $ch = curl_init();
  
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_TIMEOUT, $timeout);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
  curl_setopt($ch, CURLOPT_VERBOSE, 1);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
  curl_setopt($ch, CURLOPT_HEADER, 1);
  curl_setopt($ch, CURLINFO_HEADER_OUT, true);
  $ch_exec = curl_exec($ch);
  
  var_dump(curl_getinfo($ch));
  
  curl_close($ch);
  return $ch_exec;
}
