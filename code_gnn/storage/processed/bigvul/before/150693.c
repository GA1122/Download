std::unique_ptr<PageInfoUI::SecurityDescription> CreateSecurityDescription(
    PageInfoUI::SecuritySummaryColor style,
    int summary_id,
    int details_id) {
  std::unique_ptr<PageInfoUI::SecurityDescription> security_description(
      new PageInfoUI::SecurityDescription());
  security_description->summary_style = style;
  security_description->summary = l10n_util::GetStringUTF16(summary_id);
  security_description->details = l10n_util::GetStringUTF16(details_id);
  return security_description;
}
