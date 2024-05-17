void ChromeContentUtilityClient::PreSandboxStartup() {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  extensions::PreSandboxStartup();
#endif
}
