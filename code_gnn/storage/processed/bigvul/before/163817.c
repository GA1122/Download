void ChromeContentBrowserClientExtensionsPart::RenderProcessWillLaunch(
    content::RenderProcessHost* host) {
  int id = host->GetID();
  Profile* profile = Profile::FromBrowserContext(host->GetBrowserContext());

  host->AddFilter(new ChromeExtensionMessageFilter(id, profile));
  host->AddFilter(new ExtensionMessageFilter(id, profile));
  host->AddFilter(new IOThreadExtensionMessageFilter(id, profile));
  host->AddFilter(new ExtensionsGuestViewMessageFilter(id, profile));
  if (extensions::ExtensionsClient::Get()
          ->ExtensionAPIEnabledInExtensionServiceWorkers()) {
    host->AddFilter(new ExtensionServiceWorkerMessageFilter(
        id, profile, host->GetStoragePartition()->GetServiceWorkerContext()));
  }
}
