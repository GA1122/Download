WebUIFactoryFunction GetWebUIFactoryFunction(content::WebUI* web_ui,
                                             Profile* profile,
                                             const GURL& url) {
  if (NeedsExtensionWebUI(web_ui, profile, url))
    return &NewWebUI<ExtensionWebUI>;

  if (!url.SchemeIs(chrome::kChromeDevToolsScheme) &&
      !url.SchemeIs(chrome::kChromeInternalScheme) &&
      !url.SchemeIs(chrome::kChromeUIScheme)) {
    return NULL;
  }

  if (url.host() == chrome::kChromeUINewTabHost ||
      url.SchemeIs(chrome::kChromeInternalScheme)) {
    return &NewWebUI<NewTabUI>;
  }

   
  if (url.host() == chrome::kChromeUIBookmarksHost)
    return &NewWebUI<BookmarksUI>;
  if (url.host() == chrome::kChromeUICloudPrintResourcesHost)
    return &NewWebUI<ExternalHtmlDialogUI>;
  if (url.host() == chrome::kChromeUICloudPrintSetupHost)
    return &NewWebUI<HtmlDialogUI>;
  if (url.spec() == chrome::kChromeUIConstrainedHTMLTestURL)
    return &NewWebUI<ConstrainedHtmlUI>;
  if (url.host() == chrome::kChromeUICrashesHost)
    return &NewWebUI<CrashesUI>;
  if (url.host() == chrome::kChromeUIDevToolsHost)
    return &NewWebUI<DevToolsUI>;
  if (url.host() == chrome::kChromeUIDialogHost)
    return &NewWebUI<ConstrainedHtmlUI>;
  if (url.host() == chrome::kChromeUIExtensionsFrameHost)
    return &NewWebUI<ExtensionsUI>;
  if (url.host() == chrome::kChromeUIFeedbackHost)
    return &NewWebUI<FeedbackUI>;
  if (url.host() == chrome::kChromeUIFlagsHost)
    return &NewWebUI<FlagsUI>;
  if (url.host() == chrome::kChromeUIFlashHost)
    return &NewWebUI<FlashUI>;
  if (url.host() == chrome::kChromeUIGpuInternalsHost)
    return &NewWebUI<GpuInternalsUI>;
  if (url.host() == chrome::kChromeUIHelpFrameHost)
    return &NewWebUI<HelpUI>;
  if (url.host() == chrome::kChromeUIHistoryFrameHost)
    return &NewWebUI<HistoryUI>;
  if (url.host() == chrome::kChromeUIMediaInternalsHost)
    return &NewWebUI<MediaInternalsUI>;
  if (url.host() == chrome::kChromeUINetInternalsHost)
    return &NewWebUI<NetInternalsUI>;
  if (url.host() == chrome::kChromeUINetworkActionPredictorHost)
    return &NewWebUI<NetworkActionPredictorUI>;
  if (url.host() == chrome::kChromeUIOmniboxHost)
    return &NewWebUI<OmniboxUI>;
  if (url.host() == chrome::kChromeUIPluginsHost)
    return &NewWebUI<PluginsUI>;
  if (url.host() == chrome::kChromeUIProfilerHost)
    return &NewWebUI<ProfilerUI>;
  if (url.host() == chrome::kChromeUIQuotaInternalsHost)
    return &NewWebUI<QuotaInternalsUI>;
  if (url.host() == chrome::kChromeUISessionsHost)
    return &NewWebUI<SessionsUI>;
  if (url.host() == chrome::kChromeUISyncInternalsHost)
    return &NewWebUI<SyncInternalsUI>;
  if (url.host() == chrome::kChromeUISyncResourcesHost)
    return &NewWebUI<HtmlDialogUI>;
  if (url.host() == chrome::kChromeUITaskManagerHost)
    return &NewWebUI<TaskManagerUI>;
  if (url.host() == chrome::kChromeUITracingHost)
    return &NewWebUI<TracingUI>;
  if (url.host() == chrome::kChromeUIUberFrameHost)
    return &NewWebUI<UberFrameUI>;
  if (url.host() == chrome::kChromeUIUberHost)
    return &NewWebUI<UberUI>;
  if (url.host() == chrome::kChromeUIWorkersHost)
    return &NewWebUI<WorkersUI>;

   
#if !defined(OS_ANDROID)
  if (url.host() == chrome::kChromeUIDownloadsHost)
    return &NewWebUI<DownloadsUI>;
  if (url.host() == chrome::kChromeUISettingsFrameHost)
    return &NewWebUI<options2::OptionsUI>;
  if (url.host() == chrome::kChromeUISettingsHost)
    return &NewWebUI<OptionsUI>;
  if (url.host() == chrome::kChromeUIPrintHost &&
      !g_browser_process->local_state()->GetBoolean(
          prefs::kPrintPreviewDisabled)) {
    return &NewWebUI<PrintPreviewUI>;
  }
#endif
#if defined(OS_WIN)
  if (url.host() == chrome::kChromeUIConflictsHost)
    return &NewWebUI<ConflictsUI>;
#endif
#if (defined(USE_NSS) || defined(USE_OPENSSL)) && defined(USE_AURA)
  if (url.host() == chrome::kChromeUICertificateViewerHost)
    return &NewWebUI<CertificateViewerUI>;
#endif
#if defined(OS_CHROMEOS)
  if (url.host() == chrome::kChromeUIActiveDownloadsHost)
    return &NewWebUI<ActiveDownloadsUI>;
  if (url.host() == chrome::kChromeUIChooseMobileNetworkHost)
    return &NewWebUI<chromeos::ChooseMobileNetworkUI>;
  if (url.host() == chrome::kChromeUIImageBurnerHost)
    return &NewWebUI<ImageBurnUI>;
  if (url.host() == chrome::kChromeUIKeyboardOverlayHost)
    return &NewWebUI<KeyboardOverlayUI>;
  if (url.host() == chrome::kChromeUIMobileSetupHost)
    return &NewWebUI<MobileSetupUI>;
  if (url.host() == chrome::kChromeUIOobeHost)
    return &NewWebUI<chromeos::OobeUI>;
  if (url.host() == chrome::kChromeUIProxySettingsHost)
    return &NewWebUI<chromeos::ProxySettingsUI>;
  if (url.host() == chrome::kChromeUIRegisterPageHost)
    return &NewWebUI<RegisterPageUI>;
  if (url.host() == chrome::kChromeUISimUnlockHost)
    return &NewWebUI<chromeos::SimUnlockUI>;
  if (url.host() == chrome::kChromeUISystemInfoHost)
    return &NewWebUI<SystemInfoUI>;
#endif   

   
#if defined(ENABLE_CONFIGURATION_POLICY)
  if (url.host() == chrome::kChromeUIPolicyHost)
    return &NewWebUI<PolicyUI>;
#endif
#if defined(USE_VIRTUAL_KEYBOARD)
  if (url.host() == chrome::kChromeUIKeyboardHost)
    return &NewWebUI<KeyboardUI>;
#endif

#if (defined(OS_LINUX) && defined(TOOLKIT_VIEWS)) || defined(USE_AURA)
  if (url.host() == chrome::kChromeUICollectedCookiesHost ||
      url.host() == chrome::kChromeUIHttpAuthHost ||
      url.host() == chrome::kChromeUITabModalConfirmDialogHost) {
    return &NewWebUI<ConstrainedHtmlUI>;
  }
#endif

#if !defined(OS_CHROMEOS) && !defined(OS_ANDROID)
  if (url.host() == chrome::kChromeUISyncPromoHost) {
    if (SyncPromoUI::ShouldShowSyncPromo(profile))
      return &NewWebUI<SyncPromoUI>;
    else
      return &NewWebUI<NewTabUI>;
  }
#endif

  if (url.host() == chrome::kChromeUIChromeURLsHost ||
      url.host() == chrome::kChromeUICreditsHost ||
      url.host() == chrome::kChromeUIDNSHost ||
      url.host() == chrome::kChromeUIHistogramsHost ||
      url.host() == chrome::kChromeUIMemoryHost ||
      url.host() == chrome::kChromeUIMemoryRedirectHost ||
      url.host() == chrome::kChromeUIStatsHost ||
      url.host() == chrome::kChromeUITaskManagerHost ||
      url.host() == chrome::kChromeUITermsHost ||
      url.host() == chrome::kChromeUIVersionHost
#if defined(USE_TCMALLOC)
      || url.host() == chrome::kChromeUITCMallocHost
#endif
#if defined(OS_LINUX) || defined(OS_OPENBSD)
      || url.host() == chrome::kChromeUILinuxProxyConfigHost
      || url.host() == chrome::kChromeUISandboxHost
#endif
#if defined(OS_CHROMEOS)
      || url.host() == chrome::kChromeUICryptohomeHost
      || url.host() == chrome::kChromeUIDiscardsHost
      || url.host() == chrome::kChromeUINetworkHost
      || url.host() == chrome::kChromeUIOSCreditsHost
#endif
      ) {
    return &NewWebUI<AboutUI>;
  }

  if (url.host() == chrome::kChromeUIExtensionActivityHost &&
      CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableExtensionActivityUI)) {
    return &NewWebUI<ExtensionActivityUI>;
  }

  DLOG(WARNING) << "Unknown WebUI:" << url;
  return NULL;
}
