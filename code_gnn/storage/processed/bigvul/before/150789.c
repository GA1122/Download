bool IsRequestScanOptionsInvalid(
    const blink::mojom::WebBluetoothRequestLEScanOptionsPtr& options) {
  if (options->accept_all_advertisements)
    return options->filters.has_value();

  return HasEmptyOrInvalidFilter(options->filters);
}
