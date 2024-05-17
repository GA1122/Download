void PageInfoBubbleView::SetIdentityInfo(const IdentityInfo& identity_info) {
  std::unique_ptr<PageInfoUI::SecurityDescription> security_description =
      identity_info.GetSecurityDescription();

  summary_text_ = security_description->summary;
  GetBubbleFrameView()->UpdateWindowTitle();
  if (ui::MaterialDesignController::IsSecondaryUiMaterial()) {
    int text_style = views::style::STYLE_PRIMARY;
    switch (security_description->summary_style) {
      case SecuritySummaryColor::RED:
        text_style = STYLE_RED;
        break;
      case SecuritySummaryColor::GREEN:
        text_style = STYLE_GREEN;
        break;
    }
    static_cast<views::Label*>(GetBubbleFrameView()->title())
        ->SetEnabledColor(views::style::GetColor(
            *this, views::style::CONTEXT_DIALOG_TITLE, text_style));
  }

  if (identity_info.certificate) {
    certificate_ = identity_info.certificate;

    if (identity_info.show_ssl_decision_revoke_button) {
      header_->AddResetDecisionsLabel();
    }

    const bool valid_identity =
        (identity_info.identity_status != PageInfo::SITE_IDENTITY_STATUS_ERROR);
    const base::string16 link_title = l10n_util::GetStringUTF16(
        valid_identity ? IDS_PAGE_INFO_CERTIFICATE_VALID_LINK
                       : IDS_PAGE_INFO_CERTIFICATE_INVALID_LINK);

    views::Link* certificate_viewer_link = new views::Link(link_title);
    certificate_viewer_link->set_id(
        PageInfoBubbleView::VIEW_ID_PAGE_INFO_LINK_CERTIFICATE_VIEWER);
    certificate_viewer_link->set_listener(this);
    certificate_viewer_link->SetUnderline(false);
    if (valid_identity) {
      certificate_viewer_link->SetTooltipText(l10n_util::GetStringFUTF16(
          IDS_PAGE_INFO_CERTIFICATE_VALID_LINK_TOOLTIP,
          base::UTF8ToUTF16(certificate_->issuer().GetDisplayName())));
    } else {
      certificate_viewer_link->SetTooltipText(l10n_util::GetStringUTF16(
          IDS_PAGE_INFO_CERTIFICATE_INVALID_LINK_TOOLTIP));
    }

    site_settings_view_->AddChildView(CreateInspectLinkSection(
        PageInfoUI::GetCertificateIcon(), IDS_PAGE_INFO_CERTIFICATE,
        certificate_viewer_link));
  }

  if (identity_info.show_change_password_buttons) {
    header_->AddPasswordReuseButtons();
  }

  header_->SetDetails(security_description->details);

  Layout();
  SizeToContents();
}
