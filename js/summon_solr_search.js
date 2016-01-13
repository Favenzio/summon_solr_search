/* 
 * Summon Solr Search Javascript functions to retrieve and display results
 * from the different Search Queries.
 */
(function ($) {  
  Drupal.behaviors.summon_solr_search = {
    attach: function (context, settings) {            
      // All our js code here
      $.each(Drupal.settings.summon_solr_search, function(index, id_keywords) {
        var temp_url = id_keywords.split("|"); 
        var onesearch_query_url = '/onesearch_query/' + temp_url[0] + '/' + temp_url[1];
        $('#edit-onesearch-block-input').val(temp_url[1]);
        $.get(onesearch_query_url, function( results_html ) {
          $('#' + temp_url[0]).append(results_html);
          $('#ajax-loader-' + temp_url[0]).empty();
        });
        
        //all search handler
        $('#summon-search-all').click(function() {
          $('#1').empty();
          $("#summon-search-menu>ul>li>a.ss-active").removeClass("ss-active");
          $("#summon-search-all").addClass("ss-active");
          var article_query_url = '/onesearch_query/1/' + temp_url[1];
          $.get(article_query_url, function( results_html ) {
            $('#' + temp_url[0]).append(results_html);
          });
        });
        
        //article search handler
        $('#summon-search-articles').click(function() {
          $('#1').empty();
          $("#summon-search-menu>ul>li>a.ss-active").removeClass("ss-active");
          $("#summon-search-articles").addClass("ss-active");
          var article_query_url = '/onesearch_query/2/' + temp_url[1];
          $.get(article_query_url, function( results_html ) {
            $('#' + temp_url[0]).append(results_html);
          });
        });      

        //book search handler
        $('#summon-search-books').click(function() {
          $('#1').empty();
          $("#summon-search-menu>ul>li>a.ss-active").removeClass("ss-active");
          $("#summon-search-books").addClass("ss-active");
          var article_query_url = '/onesearch_query/3/' + temp_url[1];
          $.get(article_query_url, function( results_html ) {
            $('#' + temp_url[0]).append(results_html);
          });
        });
        
        //journal search handler
        $('#summon-search-journals').click(function() {
          $('#1').empty();
          $("#summon-search-menu>ul>li>a.ss-active").removeClass("ss-active");
          $("#summon-search-journals").addClass("ss-active");
          var article_query_url = '/onesearch_query/4/' + temp_url[1];
          $.get(article_query_url, function( results_html ) {
            $('#' + temp_url[0]).append(results_html);
          });
        });
        
        //database search handler
        $('#summon-search-databases').click(function() {
          $('#1').empty();
          $("#summon-search-menu>ul>li>a.ss-active").removeClass("ss-active");
          $("#summon-search-databases").addClass("ss-active");
          var article_query_url = '/onesearch_query/5/' + temp_url[1];
          $.get(article_query_url, function( results_html ) {
            $('#' + temp_url[0]).append(results_html);
          });
        });
        
        //website search handler
        $('#summon-search-website').click(function() {
          $('#1').empty();
          $("#summon-search-menu>ul>li>a.ss-active").removeClass("ss-active");
          $("#summon-search-website").addClass("ss-active");
          var article_query_url = '/onesearch_query/6/' + temp_url[1];
          $.get(article_query_url, function( results_html ) {
            $('#' + temp_url[0]).append(results_html);
          });
        }); 
      });
     // end our js code
    }
  };


  
})(jQuery);
 

