void WebsiteSettingsPopupView::SetIdentityInfo(
    const IdentityInfo& identity_info) {
  base::string16 identity_status_text = identity_info.GetSecuritySummary();
  header_->SetIdentityName(base::UTF8ToUTF16(identity_info.site_identity));
  header_->SetIdentityStatus(identity_status_text, SK_ColorBLACK);

  base::string16 headline;
  if (identity_info.cert_id) {
    cert_id_ = identity_info.cert_id;

    certificate_dialog_link_ = new views::Link(
        l10n_util::GetStringUTF16(IDS_PAGEINFO_CERT_INFO_BUTTON));
    certificate_dialog_link_->set_listener(this);

    if (identity_info.show_ssl_decision_revoke_button) {
      reset_decisions_button_ = new views::LabelButton(
          this,
          l10n_util::GetStringUTF16(
              IDS_PAGEINFO_RESET_INVALID_CERTIFICATE_DECISIONS_BUTTON));
      reset_decisions_button_->SetStyle(views::Button::STYLE_BUTTON);
    }

    headline = base::UTF8ToUTF16(identity_info.site_identity);
  }
  ResetConnectionSection(
      identity_info_content_,
      WebsiteSettingsUI::GetIdentityIcon(identity_info.identity_status),
      base::string16(),   
      base::UTF8ToUTF16(identity_info.identity_status_description),
      certificate_dialog_link_,
      reset_decisions_button_);

  ResetConnectionSection(
      connection_info_content_,
      WebsiteSettingsUI::GetConnectionIcon(identity_info.connection_status),
      base::string16(),   
      base::UTF8ToUTF16(identity_info.connection_status_description),
      nullptr,
      nullptr);

  connection_tab_->InvalidateLayout();
  Layout();
  SizeToContents();
}
