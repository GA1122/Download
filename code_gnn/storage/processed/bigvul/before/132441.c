void UsbDeviceImpl::RefreshConfiguration() {
  libusb_config_descriptor* platform_config;
  int rv =
      libusb_get_active_config_descriptor(platform_device_, &platform_config);
  if (rv != LIBUSB_SUCCESS) {
    USB_LOG(EVENT) << "Failed to get config descriptor: "
                   << ConvertPlatformUsbErrorToString(rv);
    return;
  }

  configuration_.reset(new UsbConfigDescriptor());
  configuration_->configuration_value = platform_config->bConfigurationValue;
  configuration_->self_powered = (platform_config->bmAttributes & 0x40) != 0;
  configuration_->remote_wakeup = (platform_config->bmAttributes & 0x20) != 0;
  configuration_->maximum_power = platform_config->MaxPower * 2;

  for (size_t i = 0; i < platform_config->bNumInterfaces; ++i) {
    const struct libusb_interface* platform_interface =
        &platform_config->interface[i];
    for (int j = 0; j < platform_interface->num_altsetting; ++j) {
      const struct libusb_interface_descriptor* platform_alt_setting =
          &platform_interface->altsetting[j];
      UsbInterfaceDescriptor interface;

      interface.interface_number = platform_alt_setting->bInterfaceNumber;
      interface.alternate_setting = platform_alt_setting->bAlternateSetting;
      interface.interface_class = platform_alt_setting->bInterfaceClass;
      interface.interface_subclass = platform_alt_setting->bInterfaceSubClass;
      interface.interface_protocol = platform_alt_setting->bInterfaceProtocol;

      for (size_t k = 0; k < platform_alt_setting->bNumEndpoints; ++k) {
        const struct libusb_endpoint_descriptor* platform_endpoint =
            &platform_alt_setting->endpoint[k];
        UsbEndpointDescriptor endpoint;

        endpoint.address = platform_endpoint->bEndpointAddress;
        endpoint.direction = GetDirection(platform_endpoint);
        endpoint.maximum_packet_size = platform_endpoint->wMaxPacketSize;
        endpoint.synchronization_type =
            GetSynchronizationType(platform_endpoint);
        endpoint.transfer_type = GetTransferType(platform_endpoint);
        endpoint.usage_type = GetUsageType(platform_endpoint);
        endpoint.polling_interval = platform_endpoint->bInterval;
        endpoint.extra_data = std::vector<uint8_t>(
            platform_endpoint->extra,
            platform_endpoint->extra + platform_endpoint->extra_length);

        interface.endpoints.push_back(endpoint);
      }

      interface.extra_data = std::vector<uint8_t>(
          platform_alt_setting->extra,
          platform_alt_setting->extra + platform_alt_setting->extra_length);

      configuration_->interfaces.push_back(interface);
    }
  }

  configuration_->extra_data = std::vector<uint8_t>(
      platform_config->extra,
      platform_config->extra + platform_config->extra_length);

  libusb_free_config_descriptor(platform_config);
}
