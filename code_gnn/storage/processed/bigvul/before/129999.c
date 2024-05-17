void ChromeBrowserMainParts::PreProfileInit() {
  TRACE_EVENT0("startup", "ChromeBrowserMainParts::PreProfileInit");

  for (size_t i = 0; i < chrome_extra_parts_.size(); ++i)
    chrome_extra_parts_[i]->PreProfileInit();

#if !defined(OS_ANDROID)
  feedback::FeedbackProfileObserver::Initialize();

  g_browser_process->profile_manager()->CleanUpEphemeralProfiles();
#endif   

#if defined(ENABLE_EXTENSIONS)
  javascript_dialog_extensions_client::InstallClient();
#endif   

#if !defined(OS_IOS)
  InstallChromeJavaScriptNativeDialogFactory();
#endif   
}
