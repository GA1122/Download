    AddDNSCertProvenanceCheckingWarningInfoBarIfNecessary(
        TabContentsWrapper* tab) {
  if (!command_line_.HasSwitch(switches::kEnableDNSCertProvenanceChecking))
    return;

  const char* kLearnMoreURL =
      "http://dev.chromium.org/dnscertprovenancechecking";
  string16 message = l10n_util::GetStringUTF16(
      IDS_DNS_CERT_PROVENANCE_CHECKING_WARNING_MESSAGE);
  tab->infobar_tab_helper()->AddInfoBar(
      new LearnMoreInfoBar(tab->infobar_tab_helper(),
                           message,
                           GURL(kLearnMoreURL)));
}
