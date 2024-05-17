void ChromeContentUtilityClient::PreSandboxStartup() {
#if defined(ENABLE_EXTENSIONS)
  extensions::ExtensionsHandler::PreSandboxStartup();
#endif

#if defined(ENABLE_MDNS)
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kUtilityProcessEnableMDns)) {
    local_discovery::ServiceDiscoveryMessageHandler::PreSandboxStartup();
  }
#endif   
}
