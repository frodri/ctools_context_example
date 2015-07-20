<?php

$plugin = array(
  'title' => t("Dota 2 match"),
  'description' => t('A container containing details on a Dota 2 match'),
  'context' => 'ctools_context_example_context_create_dota2match',  
  'context name' => 'dota2match',
  'edit form' => 'dota2match_settings_form',
  'keyword' => 'match',

  'convert list' => 'dota2match_convert_list',
  'convert' => 'dota2match_convert',

  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter a match id'),
  ),
);


function dota2match_get_steam_match_data($match_id, $api_key) {
  $response = FALSE;

  $match_request = drupal_http_request(
    url(
      'http://api.steampowered.com/IDOTA2Match_570/GetMatchDetails/v1',
      array('query' => array('match_id' => $match_id, 'key' => $api_key))
    )
  );
  $hero_request = drupal_http_request(
    url(
      'http://api.steampowered.com/IEconDOTA2_570/GetHeroes/v1',
      array('query' => array('language' => 'en', 'key' => $api_key))
    )
  );

  if ($hero_request->code != 200 || $match_request->code != 200) {
    drupal_set_message(t('Steam API is unavailable.'), 'error');
  } else {
    $response['heroes'] = $hero_request->data;
    $response['match'] = $match_request->data;
  }
  return $response;
}

function dota2match_build_data($match_id = '') {
  $data = new stdClass();
  $api_key = variable_get('ctools_context_example_api_key', '');
  $response = FALSE;

  if ($match_id != '' && $api_key != '') {
    $response = dota2match_get_steam_match_data($match_id, $api_key);
    if ($response) {
       $heroes = $response['heroes'];
       $match = $response['match'];
    }

  }

  if(!$response) {
    $path = drupal_get_path('module', 'ctools_context_example') . "/sample-data";

    // TODO: there's probably a cleaner way to do this: maybe a stream?
    $heroes = file_get_contents(
      drupal_realpath($path) . "/heroes.json"
    );
    $match = file_get_contents(
      drupal_realpath($path) . "/match.json"
    ); 
  } 

  // API Ref: https://wiki.teamfortress.com/wiki/WebAPI/GetHeroes
  // TODO: Store hero data on cache. This doesn't change very often.
  $hero_data = drupal_json_decode($heroes);
  foreach ($hero_data['result']['heroes'] as $hero) {
    $hero_data[$hero['id']] = $hero['localized_name'];
  }

  // Import the json request into the context object that the content panes
  // can use.
  // API Ref: https://wiki.teamfortress.com/wiki/WebAPI/GetMatchDetails
  $match_data = drupal_json_decode($match);
  $data->match_id = $match_data['result']['match_id'];
  $data->winner = $match_data['result']['radiant_win'] ? 'Radiant' : 'Dire';
  foreach ($match_data['result']['players'] as $player) {
    if($player['player_slot'] >= 0 && $player['player_slot'] <= 4) {
      $faction = 'radiant';
    }
    if($player['player_slot'] >= 128 && $player['player_slot'] <= 132) {
      $faction = 'dire';
    }
    $data->{$faction}['players'][] = array(
      'hero' => $hero_data[$player['hero_id']],
      'level' => $player['level'],
      'kills' => $player['kills'],
      'deaths' => $player['deaths'],
      'assists' => $player['assists'],
      'last_hits' => $player['last_hits'],
      'denies' => $player['denies'],
    );
  }


  // Tower and barracks status is stored as two integers.
  // We do some bitwise operations to get the data into an array
  $tower_status = array(
    "ancient_top" => pow(2,10),
    "ancient_bottom" => pow(2,9),
    "bottom_tier_3" => pow(2,8),
    "bottom_tier_2" => pow(2,7),
    "bottom_tier_1" => pow(2,6),
    "middle_tier_3" => pow(2,5),
    "middle_tier_2" => pow(2,4),
    "middle_tier_1" => pow(2,3),
    "top_tier_3" => pow(2,2),
    "top_tier_2" => pow(2,1),
    "top_tier_1" => pow(2,0),
  );

  foreach ($tower_status as $tower_name => $int) {
    $data->radiant['buildings'][$tower_name] = ($match_data['result']['tower_status_radiant'] & $int) > 0;
    $data->dire['buildings'][$tower_name] = ($match_data['result']['tower_status_dire'] & $int) > 0;
  }

  $barracks_status = array(
    "bottom_ranged_rax" => pow(2,5),
    "bottom_melee_rax" => pow(2,4),
    "middle_ranged_rax" => pow(2,3),
    "middle_melee_rax" => pow(2,2),
    "top_ranged_rax" => pow(2,1),
    "top_melee_rax" => pow(2,0),
  );

  foreach ($barracks_status as $barracks_name => $int) {
    $data->radiant['buildings'][$barracks_name] = ($match_data['result']['barracks_status_radiant'] & $int) > 0;
    $data->dire['buildings'][$barracks_name] = ($match_data['result']['barracks_status_dire'] & $int) > 0;
  }

  return $data;

}


function ctools_context_example_context_create_dota2match($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('dota2match');
  $context->plugin = 'dota2match';

  if ($empty) {
    return $context;
  }


  if ($conf) {
    if (!empty($data)) {
      $context->data = dota2match_build_data($data['match_id']);

      $context->title = t("dota2match context built from configuration");
      return $context;
    }
  }
  else {

    $context->title = $data;
    $context->argument = $data;

    $context->data = dota2match_build_data($context->argument);
    $context->arg_length = strlen($context->argument);
    return $context;
  }
}

function dota2match_settings_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['match_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Match ID'),
    '#size' => 25,
    '#description' => t('Enter the match id you want the Dota 2 match context to represent.'),
    '#default_value' => $conf['match_id'],
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );
  return $form;
}

function dota2match_settings_form_submit($form, &$form_state) {
  $form_state['conf']['match_id'] = $form_state['values']['match_id'];
}


function dota2match_convert_list() {
  return array(
    'match_id' => t('Match ID'),
    'winner' => t('Winner'),
  );
}

function dota2match_convert($context, $type) {
  switch ($type) {
    case 'match_id':
      return $context->data->match_id;
    case 'winner':
      return $context->data->winner;
  }
}