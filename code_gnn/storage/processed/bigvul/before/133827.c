ChromeContentUtilityClient::ChromeContentUtilityClient()
    : filter_messages_(false) {
#if !defined(OS_ANDROID)
  handlers_.push_back(new ProfileImportHandler());
#endif

#if defined(ENABLE_EXTENSIONS)
  handlers_.push_back(new extensions::ExtensionsHandler());
  handlers_.push_back(new image_writer::ImageWriterHandler());
#endif

#if defined(ENABLE_PRINT_PREVIEW) || defined(OS_WIN)
  handlers_.push_back(new PrintingHandler());
#endif

#if defined(ENABLE_MDNS)
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kUtilityProcessEnableMDns)) {
    handlers_.push_back(new local_discovery::ServiceDiscoveryMessageHandler());
  }
#endif

#if defined(OS_WIN)
  handlers_.push_back(new ShellHandler());
  handlers_.push_back(new FontCacheHandler());
#endif

  handlers_.push_back(new SafeJsonParserHandler());
}
