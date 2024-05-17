std::unique_ptr<service_manager::Service> CreatePrefService(
    base::RepeatingCallback<prefs::InProcessPrefServiceFactory*()>
        factory_callback) {
  auto* factory = factory_callback.Run();

  if (!factory)
    return std::make_unique<service_manager::Service>();

  return factory->CreatePrefService();
}
