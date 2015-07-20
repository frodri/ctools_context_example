# Ctools Context Example

This repository contains a Drupal module demonstrating how a Ctools context can consume external data (in this case, that external data would be a [Dota 2](http://dota2.com/) match) to populate the panes of a panel page.

## Instructions for previewing

* After installing the module, a new Context will be made available to the Panels interface (Dota 2 match). You may add this context directly in the Contexts section inside your Panel variant, or you can add an Argument, and assign the "Dota 2 Match ID" context from there. Once either one of those is done, you will be able to add the Panel panes included in this module (the section containing these panes is called 'Ctools Context Example') into your Page.
* For demonstration purposes, some sample API responses has been included in this module - these are automatically loaded if a) your API requests fail due to an invalid match id/api key, b) your context does not contain a match id, or c) a Steam API key is not configured in the module settings. (keys can be obtained at steamcommunity.com/dev/apikey)

## Known Issues

* This module has no caching / data storage for its API calls. In a production site, you will want to implement these.
* Error handling for the API calls is very basic at the moment.