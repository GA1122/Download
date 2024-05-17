void ChromeBrowserMainParts::PostProfileInit() {
  TRACE_EVENT0("startup", "ChromeBrowserMainParts::PostProfileInit");

#if defined(OS_ANDROID)
  DevToolsDiscoveryProviderAndroid::Install();
#else
  ChromeDevToolsDiscoveryProvider::Install();
#endif   

  LaunchDevToolsHandlerIfNeeded(parsed_command_line());
  for (size_t i = 0; i < chrome_extra_parts_.size(); ++i)
    chrome_extra_parts_[i]->PostProfileInit();
}
