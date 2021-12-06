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

        if (temp_url[2] == 'database') {
          var onesearch_query_url = '/onesearch_query/5/' + temp_url[1];
          $(".ss-active").removeClass("ss-active");
          $(".summon-search-databases").addClass("ss-active");
        }

        if (temp_url[2] == 'website') {
          var onesearch_query_url = '/onesearch_query/6/' + temp_url[1];
          $(".ss-active").removeClass("ss-active");
          $(".summon-search-website").addClass("ss-active");
        }

        $('#edit-onesearch-block-input').val(temp_url[1]);
        $.get(onesearch_query_url, function( results_html ) {
          $('#' + temp_url[0]).append(results_html);
          $('#ajax-loader-' + temp_url[0]).empty();
          fsuthemeTooltip($);
        });

        //database search handler
        $('.summon-search-databases').click(function() {
          var query_url = 'https://guides.lib.fsu.edu/az.php?q=' + temp_url[1];
	  window.location = query_url;
        });

	//website search handler
        $('.summon-search-website').click(function() {
          $('#1').empty();
          $('#1').append('<div id="load-wait"><img src="/sites/all/modules/summon_solr_search/images/ajax-loader.gif"></div>');
          $(".ss-active").removeClass("ss-active");
          $(".summon-search-website").addClass("ss-active");
          var article_query_url = '/onesearch_query/6/' + temp_url[1];
          $.get(article_query_url, function( results_html ) {
            $('#' + temp_url[0]).append(results_html);
            $('#load-wait').empty();
          });
	    });

        //additional website content handler
        $('.summon-search-container').on('click', '.summon-search-website', function() {
          $('#1').empty();
          $('#1').append('<div id="load-wait"><img src="/sites/all/modules/summon_solr_search/images/ajax-loader.gif"></div>');
          $(".ss-active").removeClass("ss-active");
          $(".summon-search-website").addClass("ss-active");
          var article_query_url = '/onesearch_query/6/' + temp_url[1];
          $.get(article_query_url, function( results_html ) {
            $('#' + temp_url[0]).append(results_html);
            $('#load-wait').empty();
          });
        });
      });
     // end our js code
    }
  };

})(jQuery);


