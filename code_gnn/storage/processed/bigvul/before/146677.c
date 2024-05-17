void PageInfoBubbleView::SetCookieInfo(const CookieInfoList& cookie_info_list) {
  if (cookie_dialog_link_ == nullptr) {
    cookie_dialog_link_ = new views::Link(
        l10n_util::GetPluralStringFUTF16(IDS_PAGE_INFO_NUM_COOKIES, 0));
    cookie_dialog_link_->set_id(
        PageInfoBubbleView::VIEW_ID_PAGE_INFO_LINK_COOKIE_DIALOG);
    cookie_dialog_link_->set_listener(this);
    cookie_dialog_link_->SetUnderline(false);

    PageInfoUI::PermissionInfo info;
    info.type = CONTENT_SETTINGS_TYPE_COOKIES;
    info.setting = CONTENT_SETTING_ALLOW;
    info.is_incognito =
        Profile::FromBrowserContext(web_contents()->GetBrowserContext())
            ->IsOffTheRecord();
    const gfx::ImageSkia icon =
        PageInfoUI::GetPermissionIcon(info).AsImageSkia();

    site_settings_view_->AddChildView(CreateInspectLinkSection(
        icon, IDS_PAGE_INFO_COOKIES, cookie_dialog_link_));
  }

  DCHECK_EQ(cookie_info_list.size(), 2u);
  int total_allowed = 0;
  for (const auto& i : cookie_info_list) {
    total_allowed += i.allowed;
  }
  base::string16 label_text = l10n_util::GetPluralStringFUTF16(
      IDS_PAGE_INFO_NUM_COOKIES, total_allowed);
  cookie_dialog_link_->SetText(label_text);
  cookie_dialog_link_->SetTooltipText(
      l10n_util::GetStringUTF16(IDS_PAGE_INFO_COOKIES_TOOLTIP));

  Layout();
  SizeToContents();
}
