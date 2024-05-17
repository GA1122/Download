views::View* CreateSiteSettingsLink(const int side_margin,
                                    views::LinkListener* listener) {
  views::Link* site_settings_link = new views::Link(
      l10n_util::GetStringUTF16(IDS_PAGE_INFO_SITE_SETTINGS_LINK));
  site_settings_link->set_id(
      PageInfoBubbleView::VIEW_ID_PAGE_INFO_LINK_SITE_SETTINGS);
  site_settings_link->SetTooltipText(
      l10n_util::GetStringUTF16(IDS_PAGE_INFO_SITE_SETTINGS_TOOLTIP));
  site_settings_link->set_listener(listener);
  site_settings_link->SetUnderline(false);
  views::View* link_section = new views::View();
  link_section->SetLayoutManager(new views::BoxLayout(
      views::BoxLayout::kHorizontal, gfx::Insets(0, side_margin)));
  link_section->AddChildView(site_settings_link);
  return link_section;
}
