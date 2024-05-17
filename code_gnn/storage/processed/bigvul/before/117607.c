void BrowserInit::LaunchWithProfile::AddBadFlagsInfoBarIfNecessary(
    TabContentsWrapper* tab) {
  static const char* kBadFlags[] = {
    switches::kSingleProcess,
    switches::kNoSandbox,
    switches::kInProcessWebGL,
    NULL
  };

  const char* bad_flag = NULL;
  for (const char** flag = kBadFlags; *flag; ++flag) {
    if (command_line_.HasSwitch(*flag)) {
      bad_flag = *flag;
      break;
    }
  }

  if (bad_flag) {
    tab->infobar_tab_helper()->AddInfoBar(
        new SimpleAlertInfoBarDelegate(
            tab->infobar_tab_helper(), NULL,
            l10n_util::GetStringFUTF16(
                IDS_BAD_FLAGS_WARNING_MESSAGE,
                UTF8ToUTF16(std::string("--") + bad_flag)),
            false));
  }
}
