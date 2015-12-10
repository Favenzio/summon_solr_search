<?php
/**
 * @file
 * Administration page callbacks for the summon solr search module.
 */
/**
 * Summon Solr Search Module administration and configuration form.
 *
 * @ingroup forms
 *
 * @return array
 *   The Drupal form definition.
 */
function summon_solr_search_admin_form() {
  $form = array();

  $form['summon_solr_search_solr_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr Host'),
    '#description' => t('IP Address or URL used for Solr Searching'),
    '#default_value' => variable_get('summon_solr_search_solr_host', 'localhost'),
  );

  $form['summon_solr_search_solr_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr Port'),
    '#description' => t('The port used to connect to the solr instance - Default is 8983'),
    '#default_value' => variable_get('summon_solr_search_solr_port', '8983'),
  );
  
  $form['summon_solr_search_solr_core'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr Core'),
    '#description' => t('The name of the Solr core to query'),
    '#default_value' => variable_get('summon_solr_search_solr_core', ''),
  );
  
  $form['summon_solr_search_summon_access_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Summon Access ID'),
    '#description' => t('The Access ID used to connect to Summon API'),
    '#default_value' => variable_get('summon_solr_search_summon_access_id', ''),
  );
  
  $form['summon_solr_search_summon_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Summon Secret Key'),
    '#description' => t('The Secret Key used to connect to Summon API'),
    '#default_value' => variable_get('summon_solr_search_summon_secret_key', ''),
  );
  
  $form['summon_solr_search_summon_api_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Summon Host URL'),
    '#description' => t('The URL to query the Summon API'),
    '#default_value' => variable_get('summon_solr_search_summon_api_host', 'api.summon.serialsolutions.com'),
  );
  
  $form['summon_solr_search_summon_api_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Summon API Path'),
    '#description' => t('The path to query from the Summon Host'),
    '#default_value' => variable_get('summon_solr_search_summon_api_path', '/2.0.0/search'),
  );

  $form['save_configuration'] = array(
    '#type' => 'submit',
    '#name' => 'saveConfiguration',
    '#default_value' => t('Save configuration'),
  );

  return $form;
}
/**
 * Submit handler for the admin form for the Summon Solr Search module.
 *
 * @param array $form
 *   An array representing a form in Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function summon_solr_search_admin_form_submit($form, &$form_state) {
  variable_set('summon_solr_search_solr_host', $form_state['values']['summon_solr_search_solr_host']);
  variable_set('summon_solr_search_solr_port', $form_state['values']['summon_solr_search_solr_port']);
  variable_set('summon_solr_search_solr_core', $form_state['values']['summon_solr_search_solr_core']);
  variable_set('summon_solr_search_summon_access_id', $form_state['values']['summon_solr_search_summon_access_id']);
  variable_set('summon_solr_search_summon_secret_key', $form_state['values']['summon_solr_search_summon_secret_key']);
  variable_set('summon_solr_search_summon_api_host', $form_state['values']['summon_solr_search_summon_api_host']);
  variable_set('summon_solr_search_summon_api_path', $form_state['values']['summon_solr_search_summon_api_path']);
}
