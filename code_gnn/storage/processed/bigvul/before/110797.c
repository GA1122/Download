void AutocompleteEditModel::OnChanged() {
  const AutocompleteMatch& current_match = user_input_in_progress_ ?
      CurrentMatch() : AutocompleteMatch();

  AutocompleteActionPredictor::Action recommended_action =
      AutocompleteActionPredictor::ACTION_NONE;
  AutocompleteActionPredictor* action_predictor =
      user_input_in_progress_ ?
      AutocompleteActionPredictorFactory::GetForProfile(profile_) : NULL;
  if (action_predictor) {
    action_predictor->RegisterTransitionalMatches(user_text_, result());
    recommended_action =
        action_predictor->RecommendAction(user_text_, current_match);
  }

  UMA_HISTOGRAM_ENUMERATION("AutocompleteActionPredictor.Action",
                            recommended_action,
                            AutocompleteActionPredictor::LAST_PREDICT_ACTION);
  string16 suggested_text;

  if (DoInstant(current_match, &suggested_text)) {
    SetSuggestedText(suggested_text, instant_complete_behavior_);
  } else {
    switch (recommended_action) {
      case AutocompleteActionPredictor::ACTION_PRERENDER:
        DoPrerender(current_match);
        break;
      case AutocompleteActionPredictor::ACTION_PRECONNECT:
        DoPreconnect(current_match);
        break;
      case AutocompleteActionPredictor::ACTION_NONE:
        break;
      default:
        NOTREACHED() << "Unexpected recommended action: " << recommended_action;
        break;
    }

    view_->SetInstantSuggestion(string16(), false);

    FinalizeInstantQuery(string16(), string16(), false);
  }

  controller_->OnChanged();
}
