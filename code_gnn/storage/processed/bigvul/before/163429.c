void OmniboxViewViews::EmphasizeURLComponents() {
  if (!location_bar_view_)
    return;

  bool text_is_url = model()->CurrentTextIsURL();
  GetRenderText()->SetDirectionalityMode(text_is_url
                                             ? gfx::DIRECTIONALITY_FORCE_LTR
                                             : gfx::DIRECTIONALITY_FROM_TEXT);
  SetStyle(gfx::STRIKE, false);
  UpdateTextStyle(text(), model()->client()->GetSchemeClassifier());
}
