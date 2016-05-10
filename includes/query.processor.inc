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
    $results = _default_query_handler($p_keywords);
    $more_results = _more_results_link('default', $p_keywords);
    return $results . $more_results;
  }
  
  if($query['query'] == 'article') {
    $results = _content_type_query_handler($p_keywords, $query['query']);
    $more_results = _more_results_link('article', $p_keywords);
    return $results . $more_results;
  }
  
  if($query['query'] == 'book') {
    $results = _content_type_query_handler($p_keywords, $query['query']);
    $more_results = _more_results_link('book', $p_keywords);
    return $results . $more_results;  
  }
  
  if($query['query'] == 'journal') {
    $results = _content_type_query_handler($p_keywords, $query['query']);
    $more_results = _more_results_link('journal', $p_keywords);
    return $results . $more_results;     
  }
  
  if($query['query'] == 'database') {
    $results = _solr_query_handler($p_keywords, 'e_resource', false);
    $more_results = _more_results_link('database', $p_keywords);
    return $results . $more_results;    
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
  
  _solr_title_search(1.5, 3, $solr_host, $solr_port, $solr_core, $keywords, $blended_results); 
  
  if(count($blended_results) < 3) {
    _solr_generic_search(0.7, $solr_host, $solr_port, $solr_core, $keywords, $blended_results);
  }
  
  $n_web_results = count($blended_results);

  _summon_generic_search($summon_auth_info, $keywords, $blended_results);
  
  $results_html = _results_array_to_html($blended_results, 'default', $n_web_results);
  
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
  
  $results_html = _results_array_to_html($blended_results, $type, 0);
  
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
  
  $results_html = _results_array_to_html($blended_results, $type, 0);
  
  return $results_html;
}

function _results_array_to_html($results, $search_type, $n_web_results) {
  $html = '';
  $type = '';
  $result_count = 1;
  
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
    elseif($result['type'] == 'staff_profile') {
      $type = 'Staff Profile';
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

    if($n_web_results != 0 && $result_count == $n_web_results) {
      $html .= '<p id="in-house-content"><a class="summon-search-website" href="#">More FSU Libraries Content</a></p>';
    }
  
    $result_count++;
  }
  
  return $html;
}

function _more_results_link($query_type, $keywords) {
  
  if($query_type == 'default') {
    $more_link = '<div id="more-results-link"><p><a href="' .
      'http://fsu.summon.serialssolutions.com/#!/search?ho=t&l=en&q=' . $keywords .
      '" target="_blank">Refine your search</a></p></div>';
    return $more_link;
  }
  
  if($query_type == 'article') {
    $more_link = '<div id="more-results-link"><p><a href="' .
      'http://fsu.summon.serialssolutions.com/#!/search?ho=t&fvf=IsPeerReviewed,true,f%7CContentType,Journal%20Article,f&l=en&q=' . 
      $keywords .'" target="_blank">Refine your search</a></p></div>';
    return $more_link;
  }
  
  if($query_type == 'book') {
    $more_link = '<div id="more-results-link"><p><a href="' .
      'http://fsu.summon.serialssolutions.com/#!/search?ho=t&fvf=ContentType,Book%20%2F%20eBook,f&l=en&q=' . 
      $keywords . '" target="_blank">Refine your search</a></p></div>';
    return $more_link;
  }
  
  if($query_type == 'journal') {
    $more_link = '<div id="more-results-link"><p><a href="' .
      'http://fsu.summon.serialssolutions.com/#!/search?ho=t&fvf=ContentType,Journal%20%2F%20eJournal,f&l=en&q=' . 
      $keywords . '" target="_blank">Refine your search</a></p></div>';
    return $more_link;
  }
  
  if($query_type == 'database') {
    $more_link = '<div id="more-results-link"><p>' .
      '<a href="/eresources/az">Databases A thru Z</a></p></div>';
    return $more_link;
  }  
}
