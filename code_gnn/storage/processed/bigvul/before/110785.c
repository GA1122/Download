void AutocompleteEditModel::DoPreconnect(const AutocompleteMatch& match) {
  if (!match.destination_url.SchemeIs(chrome::kExtensionScheme)) {
    UMA_HISTOGRAM_ENUMERATION("Autocomplete.MatchType", match.type,
                              AutocompleteMatch::NUM_TYPES);
    if (profile_->GetNetworkPredictor()) {
      profile_->GetNetworkPredictor()->AnticipateOmniboxUrl(
          match.destination_url,
          AutocompleteActionPredictor::IsPreconnectable(match));
    }
  }
}
