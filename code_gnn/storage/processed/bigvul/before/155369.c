std::vector<std::string> ChromeContentBrowserClient::GetStartupServices() {
#if defined(OS_ANDROID)
  return {"download_manager"};
#else
  return {};
#endif
}
