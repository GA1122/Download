void TabStrip::UpdateHoverCard(Tab* tab, bool should_show) {
  if (!base::FeatureList::IsEnabled(features::kTabHoverCards))
    return;

  if (!hover_card_) {
    if (!should_show)
      return;
    hover_card_ = new TabHoverCardBubbleView(tab);
    hover_card_->views::View::AddObserver(this);
    if (GetWidget()) {
      hover_card_event_sniffer_ =
          std::make_unique<TabHoverCardEventSniffer>(hover_card_, this);
    }
  }
  if (should_show)
    hover_card_->UpdateAndShow(tab);
  else
    hover_card_->FadeOutToHide();
}
