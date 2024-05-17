void OmniboxViewViews::EmphasizeURLComponents() {
  if (!location_bar_view_)
    return;

  if (path_fade_animation_)
    path_fade_animation_->Stop();

  bool text_is_url = model()->CurrentTextIsURL();
  GetRenderText()->SetDirectionalityMode(
      text_is_url ? gfx::DIRECTIONALITY_AS_URL : gfx::DIRECTIONALITY_FROM_TEXT);
  SetStyle(gfx::TEXT_STYLE_STRIKE, false);

  base::string16 text = GetText();
  bool path_eligible_for_fading = UpdateTextStyle(
      text, text_is_url, model()->client()->GetSchemeClassifier());

  if (path_fade_animation_ && path_eligible_for_fading && !HasFocus() &&
      !model()->user_input_in_progress()) {
    url::Component scheme, host;
    AutocompleteInput::ParseForEmphasizeComponents(
        text, model()->client()->GetSchemeClassifier(), &scheme, &host);
    gfx::Range path_bounds(host.end(), text.size());

    path_fade_animation_->Start(path_bounds);
  }
}
