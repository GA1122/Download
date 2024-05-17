void LogRequestDeviceOptions(
    const blink::mojom::WebBluetoothRequestDeviceOptionsPtr& options) {
  DVLOG(1) << "requestDevice called with the following filters: ";
  DVLOG(1) << "acceptAllDevices: " << options->accept_all_devices;

  if (!options->filters)
    return;

  int i = 0;
  for (const auto& filter : options->filters.value()) {
    DVLOG(1) << "Filter #" << ++i;
    if (filter->name)
      DVLOG(1) << "Name: " << filter->name.value();

    if (filter->name_prefix)
      DVLOG(1) << "Name Prefix: " << filter->name_prefix.value();

    if (filter->services) {
      DVLOG(1) << "Services: ";
      DVLOG(1) << "\t[";
      for (const auto& service : filter->services.value())
        DVLOG(1) << "\t\t" << service.canonical_value();
      DVLOG(1) << "\t]";
    }
  }
}
