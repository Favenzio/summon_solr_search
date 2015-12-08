<?php
/**
 * A Collection of functions that process queries
 */
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
 *   A result set object containing the list of search sources.
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
  
  _solr_title_search($solr_host, $solr_port, $solr_core, $parsed_keywords, $blended_results); 
  
  $results_html = _results_array_to_html($blended_results);
  
  return $results_html;
}

// Returns a JSON object with a maximum of 2 results with a relevance score higher than 1.5
function _solr_title_search($solr_host, $solr_port, $solr_core, $keywords, &$blended_results) {
  $count = 0;  
  $filter_one = 'http://' . $solr_host . ':' . $solr_port . '/solr/' . $solr_core
    . '/select?q=tm_title%3A' . $keywords 
    . '&fl=score%2Css_url%2Css_type%2Ctm_title%2Ccontent&wt=json&indent=true';
  
  $title_search_results = json_decode(file_get_contents($filter_one));
  
  foreach($title_search_results->response->docs as $result) {
    if($result->score > 1.5) {
      $blended_results[] = array (
        'type' => $result->ss_type,
        'title' => $result->tm_title[0],
        'url' => $result->ss_url,
        'content' => $result->content, 
      );
      
      $count++;
      
      if($count > 1) {
        break;
      }
    }
  }
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
