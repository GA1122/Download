OmniboxViewViews::OmniboxViewViews(OmniboxEditController* controller,
                                   std::unique_ptr<OmniboxClient> client,
                                   bool popup_window_mode,
                                   LocationBarView* location_bar,
                                   const gfx::FontList& font_list)
    : OmniboxView(controller, std::move(client)),
      popup_window_mode_(popup_window_mode),
      saved_selection_for_focus_change_(gfx::Range::InvalidRange()),
      location_bar_view_(location_bar),
      latency_histogram_state_(NOT_ACTIVE),
      friendly_suggestion_text_prefix_length_(0),
      scoped_compositor_observer_(this),
      scoped_template_url_service_observer_(this) {
  set_id(VIEW_ID_OMNIBOX);
  SetFontList(font_list);

  if (base::FeatureList::IsEnabled(
          omnibox::kHideSteadyStateUrlPathQueryAndRef)) {
    SkColor starting_color =
        location_bar_view_->GetColor(OmniboxPart::LOCATION_BAR_TEXT_DIMMED);
    path_fade_animation_ =
        std::make_unique<PathFadeAnimation>(this, starting_color);
  }
}
