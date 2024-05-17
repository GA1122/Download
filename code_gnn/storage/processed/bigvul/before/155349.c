ChromeContentBrowserClient::GetInitiatorSchemeBypassingDocumentBlocking() {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  return extensions::kExtensionScheme;
#else
  return nullptr;
#endif
}
