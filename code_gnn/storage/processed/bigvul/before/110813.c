void AutocompleteEditModel::SetSuggestedText(
    const string16& text,
    InstantCompleteBehavior behavior) {
  instant_complete_behavior_ = behavior;
  if (instant_complete_behavior_ == INSTANT_COMPLETE_NOW) {
    if (!text.empty())
      FinalizeInstantQuery(view_->GetText(), text, false);
    else
      view_->SetInstantSuggestion(text, false);
  } else {
    DCHECK((behavior == INSTANT_COMPLETE_DELAYED) ||
           (behavior == INSTANT_COMPLETE_NEVER));
    view_->SetInstantSuggestion(text, behavior == INSTANT_COMPLETE_DELAYED);
  }
}
