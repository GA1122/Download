bool TabStrip::HoverCardIsShowingForTab(Tab* tab) {
  if (!base::FeatureList::IsEnabled(features::kTabHoverCards))
    return false;

  return hover_card_ && hover_card_->GetWidget()->IsVisible() &&
         !hover_card_->IsFadingOut() && hover_card_->GetAnchorView() == tab;
}
