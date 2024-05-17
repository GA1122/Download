bool ChromeContentBrowserClient::ShouldSubframesTryToReuseExistingProcess(
    content::RenderFrameHost* main_frame) {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  return ChromeContentBrowserClientExtensionsPart::
      ShouldSubframesTryToReuseExistingProcess(main_frame);
#else
  return true;
#endif
}
