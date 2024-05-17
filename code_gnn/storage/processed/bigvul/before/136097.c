void WebsiteSettingsPopupView::SetCookieInfo(
    const CookieInfoList& cookie_info_list) {
  site_data_content_->RemoveAllChildViews(true);

  views::GridLayout* layout = new views::GridLayout(site_data_content_);
  site_data_content_->SetLayoutManager(layout);

  const int site_data_content_column = 0;
  views::ColumnSet* column_set =
      layout->AddColumnSet(site_data_content_column);
  column_set->AddColumn(views::GridLayout::FILL,
                        views::GridLayout::FILL,
                        1,
                        views::GridLayout::FIXED,
                        kSiteDataIconColumnWidth,
                        0);
  column_set->AddPaddingColumn(0, kIconMarginLeft);
  column_set->AddColumn(views::GridLayout::FILL,
                        views::GridLayout::FILL,
                        1,
                        views::GridLayout::USE_PREF,
                        0,
                        0);

  layout->AddPaddingRow(1, 5);
  for (CookieInfoList::const_iterator i(cookie_info_list.begin());
       i != cookie_info_list.end();
       ++i) {
    base::string16 label_text = l10n_util::GetStringFUTF16(
        IDS_WEBSITE_SETTINGS_SITE_DATA_STATS_LINE,
        base::UTF8ToUTF16(i->cookie_source),
        base::IntToString16(i->allowed),
        base::IntToString16(i->blocked));
    if (i != cookie_info_list.begin())
      layout->AddPaddingRow(1, kSiteDataSectionRowSpacing);
    layout->StartRow(1, site_data_content_column);
    WebsiteSettingsUI::PermissionInfo info;
    info.type = CONTENT_SETTINGS_TYPE_COOKIES;
    info.setting = CONTENT_SETTING_ALLOW;
    views::ImageView* icon = new views::ImageView();
    const gfx::Image& image = WebsiteSettingsUI::GetPermissionIcon(info);
    icon->SetImage(image.ToImageSkia());
    layout->AddView(icon, 1, 1, views::GridLayout::CENTER,
                    views::GridLayout::CENTER);
    layout->AddView(new views::Label(label_text), 1, 1,
                    views::GridLayout::LEADING, views::GridLayout::CENTER);
  }
  layout->AddPaddingRow(1, 6);

  layout->Layout(site_data_content_);
  SizeToContents();
}
