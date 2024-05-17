void ChromeContentUtilityClient::UtilityThreadStarted() {
#if defined(ENABLE_EXTENSIONS)
  extensions::UtilityHandler::UtilityThreadStarted();
#endif

  if (kMessageWhitelistSize > 0) {
    base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
    if (command_line->HasSwitch(switches::kUtilityProcessRunningElevated)) {
      message_id_whitelist_.insert(kMessageWhitelist,
                                   kMessageWhitelist + kMessageWhitelistSize);
      filter_messages_ = true;
    }
  }
}
