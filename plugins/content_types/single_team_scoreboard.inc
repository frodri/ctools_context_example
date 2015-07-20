<?php


$plugin = array(
  'title' => t('Dota 2 Scoreboard (Single Team)'),
  'content_types' => 'single_team_scoreboard',
  'single' => TRUE,
  'render callback' => 'single_team_scoreboard_content_type_render',

  'description' => t('A Dota 2 Scoreboard for either the Radiant or the dire. Requires a Dota 2 match context.'),
  'required context' => new ctools_context_required(t('Dota 2 match'), 'dota2match'),
  'edit form' => 'single_team_scoreboard_content_type_edit_form',
  'admin title' => 'ctools_context_example_single_team_scoreboard_content_type_admin_title',

  'admin info' => 'ctools_context_example_single_team_scoreboard_content_type_admin_info',
  'category' => array(t('CTools Context Example'), 0),
);

function ctools_context_example_single_team_scoreboard_content_type_admin_title($subtype, $conf, $context = NULL) {
  $output = t('Dota 2 Scoreboard (Single Team)');
  if ($conf['override_title'] && !empty($conf['override_title_text'])) {
    $output = filter_xss_admin($conf['override_title_text']);
  }
  return $output;
}

function ctools_context_example_single_team_scoreboard_content_type_admin_info($subtype, $conf, $context = NULL) {
  $context = new stdClass();
  $context->data = new stdClass();
  $block = single_team_scoreboard_content_type_render($subtype, $conf, array('admin-preview'), $context);
  return $block;
}


function single_team_scoreboard_content_type_render($subtype, $conf, $args, $context) {
  $data = $context->data;
  $block = new stdClass();
  

  switch ($conf['team']) {
    case 'radiant':
      $block->title = t("Radiant Team Stats");
      break;
    case 'dire':
      $block->title = t("Dire Team Stats");
      break;
  }

  if (!empty($args) && $args[0] == 'admin-preview') {
    $block->content = 'Scoreboard goes here';
  } else {
    $block->content = array(
      '#theme' => 'match_stats',
      '#stats' => $data->{$conf['team']},
    );
  }
  
    return $block;
}


function single_team_scoreboard_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['team'] = array(
    '#type' => 'select',
    '#title' => t('Team to display'),
    '#options' => array(
      'radiant' => t('Radiant'),
      'dire' => t('Dire'),
     ),
    '#description' => t('Select the team you want to display on this widget.'),
    '#default_value' => !empty($conf['team']) ? $conf['team'] : 'radiant',
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );

  return $form;
}

function single_team_scoreboard_content_type_edit_form_submit($form, &$form_state) {
  foreach (element_children($form) as $key) {
    if (!empty($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}
