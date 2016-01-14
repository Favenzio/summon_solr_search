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
  
  $p_keywords = str_replace(' ', '+', $keywords);
  
  if($query['query'] == 'default') {
    return _default_query_handler($p_keywords);
  }
  
  if($query['query'] == 'article') {
    return _content_type_query_handler($p_keywords, $query['query']);
  }
  
  if($query['query'] == 'book') {
    return _content_type_query_handler($p_keywords, $query['query']);
  }
  
  if($query['query'] == 'journal') {
    return _content_type_query_handler($p_keywords, $query['query']);
  }
  
  if($query['query'] == 'database') {
    return _solr_query_handler($p_keywords, 'e_resource', false);
  }
  
  if($query['query'] == 'website') {
    return _solr_query_handler($p_keywords, 'e_resource', true);
  }
  
  return "<p>Query not found.</p>";
}

function _default_query_handler($keywords) {
  $blended_results = array();
  
  $solr_host = variable_get('summon_solr_search_solr_host');
  $solr_port = variable_get('summon_solr_search_solr_port');
  $solr_core = variable_get('summon_solr_search_solr_core'); 
  
  $summon_auth_info = array(
    'id' => variable_get('summon_solr_search_summon_access_id'),
    'key' => variable_get('summon_solr_search_summon_secret_key'),
    'host' => variable_get('summon_solr_search_summon_api_host'),
    'path' => variable_get('summon_solr_search_summon_api_path'),
  );
  
  _solr_title_search(1.5, 2, $solr_host, $solr_port, $solr_core, $keywords, $blended_results); 
  
  if(count($blended_results) < 2) {
    _solr_generic_search(0.7, $solr_host, $solr_port, $solr_core, $keywords, $blended_results);
  }
  
  _summon_generic_search($summon_auth_info, $keywords, $blended_results);
  
  $results_html = _results_array_to_html($blended_results, 'default');
  
  return $results_html;
}

function _solr_query_handler($keywords, $type, $remove) {
  $blended_results = array();
  
  $solr_host = variable_get('summon_solr_search_solr_host');
  $solr_port = variable_get('summon_solr_search_solr_port');
  $solr_core = variable_get('summon_solr_search_solr_core'); 
  
  if($remove){
    _solr_remove_type_search($type, 10, $solr_host, $solr_port, 
      $solr_core, $keywords, $blended_results); 
  } else {
    _solr_type_search($type, 10, $solr_host, $solr_port, 
      $solr_core, $keywords, $blended_results); 
  }
  
  $results_html = _results_array_to_html($blended_results, $type);
  
  return $results_html;
}

function _content_type_query_handler($keywords, $type) {
  $blended_results = array();
  
  $summon_auth_info = array(
    'id' => variable_get('summon_solr_search_summon_access_id'),
    'key' => variable_get('summon_solr_search_summon_secret_key'),
    'host' => variable_get('summon_solr_search_summon_api_host'),
    'path' => variable_get('summon_solr_search_summon_api_path'),
  );
  
  _summon_content_type_search($type, $summon_auth_info, $keywords, $blended_results);
  
  $results_html = _results_array_to_html($blended_results, $type);
  
  return $results_html;
}

function _results_array_to_html($results, $search_type) {
  $html = '';
  $type = '';
  
  if (count($results) == 0) {
    $html = '<h5>No records found. Please enter different keywords and try again.</h5>';
    $html .= '<p>For personalized help, use our '
      . '<a href="http://askalibrarian.org/fsulibraries/">Ask a Librarian</a> service.</p>';
    return $html;
  }
  
  foreach ($results as $result) {
    if(isset($result['content'])) {
      $summary = substr(strip_tags($result['content']), 0, 300) . '...';
    }
    
    if($result['type'] == 'research_guide') {
      $type = 'Research Guide';
    }
    elseif($result['type'] == 'basic_page') {
      $type = 'Library Website';
    }
    elseif($result['type'] == 'e_resource') {
      $type = 'Database';
    }    
    elseif($result['type'] == 'service' || $result['type'] == 'policy') {
      $type = 'Library Website';
    }
    else {
      $type = $result['type'];
    }
    
    $html .= '<div id="summon-solr-result"><h3><a href="' . $result['url'] . '" target="_blank">';
    $html .= $result['title'] . '</a></h3>';

    if(isset($result['author'])){
      $html .= '<h5>' . $result['author'] . '</h5>';
    }
    
    if(isset($result['content'])){
      $html .= '<p>' . $summary . '</p>';
    }
    
    $html .= '<h6>' . $type;
    
    if($result['full_text'] == 'true') {
      $html .= ' - <a href="' . $result['url'] . '" target = "_blank">Full Text Online</a>'; 
    }
    elseif (isset($result['subject_terms'])) {
      $html .= ': ' . $result['subject_terms'];
    }
    
    $html .= '</h6></div>';
  }
  
  return $html;
}
