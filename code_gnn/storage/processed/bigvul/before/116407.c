void ChromeContentRendererClient::RegisterPPAPIInterfaceFactories(
    webkit::ppapi::PpapiInterfaceFactoryManager* factory_manager) {
  factory_manager->RegisterFactory(ChromePPAPIInterfaceFactory);
}
