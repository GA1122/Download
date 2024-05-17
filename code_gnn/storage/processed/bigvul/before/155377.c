content::TtsPlatform* ChromeContentBrowserClient::GetTtsPlatform() {
#ifdef OS_CHROMEOS
  return TtsPlatformImplChromeOs::GetInstance();
#else
  return nullptr;
#endif
}
