bool IsRequestDeviceOptionsInvalid(
    const blink::mojom::WebBluetoothRequestDeviceOptionsPtr& options) {
  if (options->accept_all_devices)
    return options->filters.has_value();

  return HasEmptyOrInvalidFilter(options->filters);
}
