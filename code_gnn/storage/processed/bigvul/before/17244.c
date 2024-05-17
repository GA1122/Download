void CreateVibrationManager(
    mojo::InterfaceRequest<device::VibrationManager> request) {
  BrowserPlatformIntegration::GetInstance()
      ->CreateVibrationManager(std::move(request));
}
