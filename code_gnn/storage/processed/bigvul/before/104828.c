ExtensionService::~ExtensionService() {

  ProviderCollection::const_iterator i;
  for (i = external_extension_providers_.begin();
       i != external_extension_providers_.end(); ++i) {
    ExternalExtensionProviderInterface* provider = i->get();
    provider->ServiceShutdown();
  }

#if defined(OS_CHROMEOS)
  if (event_routers_initialized_) {
    ExtensionFileBrowserEventRouter::GetInstance()->
        StopObservingFileSystemEvents();
  }
#endif
}
