void InitializeNetworkPortalDetector() {
  if (network_portal_detector::SetForTesting())
    return;
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          ::switches::kTestType)) {
    network_portal_detector::SetNetworkPortalDetector(
        new NetworkPortalDetectorStub());
  } else {
    network_portal_detector::SetNetworkPortalDetector(
        new NetworkPortalDetectorImpl(
            g_browser_process->system_network_context_manager()
                ->GetURLLoaderFactory()));
  }
}
