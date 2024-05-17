void OmniboxViewViews::InstallPlaceholderText() {
  set_placeholder_text_color(
      location_bar_view_->GetColor(OmniboxPart::LOCATION_BAR_TEXT_DIMMED));

  const TemplateURL* const default_provider =
      model()->client()->GetTemplateURLService()->GetDefaultSearchProvider();
  if (default_provider) {
    set_placeholder_text(l10n_util::GetStringFUTF16(
        IDS_OMNIBOX_PLACEHOLDER_TEXT, default_provider->short_name()));
  } else {
    set_placeholder_text(base::string16());
  }
}
