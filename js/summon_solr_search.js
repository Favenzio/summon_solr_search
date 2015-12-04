/* 
 * Summon Solr Search Javascript functions to retrieve and display results
 * from the different Search Queries.
 */

/* Fixed Header */
(function ($) {  
  Drupal.behaviors.summon_solr_search = {
    attach: function (context, settings) {            
     // All our js code here
     $.each(Drupal.settings.summon_solr_search, function(index, id_keywords) {
       var temp_url = id_keywords.split("|"); 
       var onesearch_query_url = '/onesearch_query/' + temp_url[0] + '/' + temp_url[1];
       $('#edit-onesearch-input').val(temp_url[1]);
       $.get(onesearch_query_url, function( results_html ) {
         $('#' + temp_url[0]).append(results_html);
         $('#ajax-loader-' + temp_url[0]).empty();
       });
    
     });
     // end our js code
   }
 };})(jQuery);
