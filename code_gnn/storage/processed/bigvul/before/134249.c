void OmniboxViewViews::EmphasizeURLComponents() {
  url_parse::Component scheme, host;
  AutocompleteInput::ParseForEmphasizeComponents(text(), &scheme, &host);
  bool grey_out_url = text().substr(scheme.begin, scheme.len) ==
      base::UTF8ToUTF16(extensions::kExtensionScheme);
  bool grey_base = model()->CurrentTextIsURL() &&
      (host.is_nonempty() || grey_out_url);
  SetColor(location_bar_view_->GetColor(
      security_level_,
      grey_base ? LocationBarView::DEEMPHASIZED_TEXT : LocationBarView::TEXT));
  if (grey_base && !grey_out_url) {
    ApplyColor(
        location_bar_view_->GetColor(security_level_, LocationBarView::TEXT),
        gfx::Range(host.begin, host.end()));
  }

  SetStyle(gfx::DIAGONAL_STRIKE, false);
  if (!model()->user_input_in_progress() && model()->CurrentTextIsURL() &&
      scheme.is_nonempty() && (security_level_ != ToolbarModel::NONE)) {
    SkColor security_color = location_bar_view_->GetColor(
        security_level_, LocationBarView::SECURITY_TEXT);
    const bool strike = (security_level_ == ToolbarModel::SECURITY_ERROR);
    const gfx::Range scheme_range(scheme.begin, scheme.end());
    ApplyColor(security_color, scheme_range);
    ApplyStyle(gfx::DIAGONAL_STRIKE, strike, scheme_range);
  }
}
