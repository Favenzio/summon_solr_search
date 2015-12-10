<?php
/**
 * A Collection of functions that process queries
 */

require_once drupal_get_path('module', 'summon_solr_search') . '/includes/solr.helper.inc';
require_once drupal_get_path('module', 'summon_solr_search') . '/includes/summon.helper.inc';

/**
 * Returns the processed html from a queried system.
 *
 * @id
 *   the id for the query
 * 
 * @keywords
 *   the keywords to use when querying the system
 * 
 * @return
 *   A string containing the HTML for the results
 */
function _process_system_query($id, $keywords) {
  $query = db_select('summon_solr_search', 'n')
    ->fields('n')
    ->condition('id', $id)
    ->execute()
    ->fetchAssoc();
  
  if($query['query'] == 'default') {
    return _default_query_handler($keywords);
  }
  
  return "<p>Query not found.</p>";
}

function _default_query_handler($keywords) {
  $blended_results = array();
  
  $parsed_keywords = str_replace(' ', '+', $keywords);
  $solr_host = variable_get('summon_solr_search_solr_host');
  $solr_port = variable_get('summon_solr_search_solr_port');
  $solr_core = variable_get('summon_solr_search_solr_core'); 
  
  $summon_auth_info = array(
    'id' => variable_get('summon_solr_search_summon_access_id'),
    'key' => variable_get('summon_solr_search_summon_secret_key'),
    'host' => variable_get('summon_solr_search_summon_api_host'),
    'path' => variable_get('summon_solr_search_summon_api_path'),
  );
  
  _solr_title_search($solr_host, $solr_port, $solr_core, $parsed_keywords, $blended_results); 
  
  if(count($blended_results) < 2) {
    _solr_generic_search($solr_host, $solr_port, $solr_core, $parsed_keywords, $blended_results);
  }
  
  _summon_generic_search($summon_auth_info, $parsed_keywords, $blended_results);
  
  $results_html = _results_array_to_html($blended_results);
  
  return $results_html;
}

function _results_array_to_html($results) {
  $html = '';
  
  foreach ($results as $result) {
    $summary = substr(strip_tags($result['content']), 0, 300);
    $html .= '<div id="summon-solr-result"><h3><a href="' . $result['url'] . '"><h3>';
    $html .= $result[title] . '</a></h3>';
    $html .= '<h5>' . $result['url'] . ' - ' . $result['type'] . '</h5>';
    $html .= '<p>' . $summary . ' ...</p>';
  }
  
  return $html;
}
