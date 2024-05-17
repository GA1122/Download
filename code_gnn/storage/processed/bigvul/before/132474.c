ExtensionFunction::ResponseAction UsbGetDevicesFunction::Run() {
  scoped_ptr<extensions::core_api::usb::GetDevices::Params> parameters =
      GetDevices::Params::Create(*args_);
  EXTENSION_FUNCTION_VALIDATE(parameters.get());

  if (parameters->options.filters) {
    filters_.resize(parameters->options.filters->size());
    for (size_t i = 0; i < parameters->options.filters->size(); ++i) {
      ConvertDeviceFilter(*parameters->options.filters->at(i).get(),
                          &filters_[i]);
    }
  }
  if (parameters->options.vendor_id) {
    filters_.resize(filters_.size() + 1);
    filters_.back().SetVendorId(*parameters->options.vendor_id);
    if (parameters->options.product_id) {
      filters_.back().SetProductId(*parameters->options.product_id);
    }
  }

  UsbService* service = device::DeviceClient::Get()->GetUsbService();
  if (!service) {
    return RespondNow(Error(kErrorInitService));
  }

  service->GetDevices(
      base::Bind(&UsbGetDevicesFunction::OnGetDevicesComplete, this));
  return RespondLater();
}
