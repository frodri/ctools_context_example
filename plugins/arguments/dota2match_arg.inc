<?php

$plugin = array(
  'title' => t("Dota 2 Match ID"),
  'keyword' => 'match',
  'description' => t('Creates a Dota 2 match context from an argument.'),
  'context' => 'dota2match_arg_context',
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter a match id.'),
  ),
);


function dota2match_arg_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  if ($empty) {
    return ctools_context_create_empty('dota2match');
  }

  if (empty($arg) || !is_numeric($arg)) {
    return FALSE;
  }
  
  return ctools_context_create('dota2match', $arg);
}
