void ReportAnyInsecureContent(
    const security_state::VisibleSecurityState& visible_security_state,
    PageInfo::SiteConnectionStatus* connection_status,
    base::string16* connection_details) {
  bool displayed_insecure_content =
      visible_security_state.displayed_mixed_content;
  bool ran_insecure_content = visible_security_state.ran_mixed_content;
  if (!net::IsCertStatusError(visible_security_state.cert_status) ||
      net::IsCertStatusMinorError(visible_security_state.cert_status)) {
    displayed_insecure_content =
        displayed_insecure_content ||
        visible_security_state.displayed_content_with_cert_errors;
    ran_insecure_content = ran_insecure_content ||
                           visible_security_state.ran_content_with_cert_errors;
  }

  if (ran_insecure_content) {
    *connection_status =
        PageInfo::SITE_CONNECTION_STATUS_INSECURE_ACTIVE_SUBRESOURCE;
    connection_details->assign(l10n_util::GetStringFUTF16(
        IDS_PAGE_INFO_SECURITY_TAB_ENCRYPTED_SENTENCE_LINK, *connection_details,
        l10n_util::GetStringUTF16(
            IDS_PAGE_INFO_SECURITY_TAB_ENCRYPTED_INSECURE_CONTENT_ERROR)));
    return;
  }
  if (visible_security_state.contained_mixed_form) {
    *connection_status = PageInfo::SITE_CONNECTION_STATUS_INSECURE_FORM_ACTION;
    connection_details->assign(l10n_util::GetStringFUTF16(
        IDS_PAGE_INFO_SECURITY_TAB_ENCRYPTED_SENTENCE_LINK, *connection_details,
        l10n_util::GetStringUTF16(
            IDS_PAGE_INFO_SECURITY_TAB_ENCRYPTED_INSECURE_FORM_WARNING)));
    return;
  }
  if (displayed_insecure_content) {
    *connection_status =
        PageInfo::SITE_CONNECTION_STATUS_INSECURE_PASSIVE_SUBRESOURCE;
    connection_details->assign(l10n_util::GetStringFUTF16(
        IDS_PAGE_INFO_SECURITY_TAB_ENCRYPTED_SENTENCE_LINK, *connection_details,
        l10n_util::GetStringUTF16(
            IDS_PAGE_INFO_SECURITY_TAB_ENCRYPTED_INSECURE_CONTENT_WARNING)));
  }
}
