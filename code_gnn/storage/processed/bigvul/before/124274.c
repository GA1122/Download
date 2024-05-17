void ExtensionSystemImpl::InitForRegularProfile(bool extensions_enabled) {
  DCHECK(!profile_->IsOffTheRecord());
  if (user_script_master() || extension_service())
    return;   

  shared_->info_map();

  extension_process_manager_.reset(ExtensionProcessManager::Create(profile_));
  alarm_manager_.reset(new AlarmManager(profile_, &base::Time::Now));

  serial_connection_manager_.reset(new ApiResourceManager<SerialConnection>(
      BrowserThread::FILE));
  socket_manager_.reset(new ApiResourceManager<Socket>(BrowserThread::IO));
  usb_device_resource_manager_.reset(
      new ApiResourceManager<UsbDeviceResource>(BrowserThread::IO));

  rules_registry_service_.reset(new RulesRegistryService(profile_));
  rules_registry_service_->RegisterDefaultRulesRegistries();

  shared_->Init(extensions_enabled);
}
