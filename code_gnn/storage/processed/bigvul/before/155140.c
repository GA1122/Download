void OmniboxViewViews::OnFocus() {
  views::Textfield::OnFocus();
  model()->ResetDisplayTexts();
  model()->OnSetFocus(false);

  if (saved_selection_for_focus_change_.IsValid()) {
    SelectRange(saved_selection_for_focus_change_);
    saved_selection_for_focus_change_ = gfx::Range::InvalidRange();
  }

  GetRenderText()->SetElideBehavior(gfx::NO_ELIDE);

  if (location_bar_view_ && model()->is_keyword_hint())
    location_bar_view_->Layout();

#if BUILDFLAG(ENABLE_DESKTOP_IN_PRODUCT_HELP)
  if (location_bar_view_ &&
      controller()->GetLocationBarModel()->ShouldDisplayURL()) {
    feature_engagement::NewTabTrackerFactory::GetInstance()
        ->GetForProfile(location_bar_view_->profile())
        ->OnOmniboxFocused();
  }
#endif

  if (location_bar_view_)
    location_bar_view_->OnOmniboxFocused();
}
