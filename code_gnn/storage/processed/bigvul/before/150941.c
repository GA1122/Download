static void ConvertRequestLEScanOptions(
    const BluetoothLEScanOptions* options,
    mojom::blink::WebBluetoothRequestLEScanOptionsPtr& result,
    ExceptionState& exception_state) {
  if (!(options->hasFilters() ^ options->acceptAllAdvertisements())) {
    exception_state.ThrowTypeError(
        "Either 'filters' should be present or 'acceptAllAdvertisements' "
        "should be true, but not both.");
    return;
  }

  result->accept_all_advertisements = options->acceptAllAdvertisements();
  result->keep_repeated_devices = options->keepRepeatedDevices();

  if (options->hasFilters()) {
    if (options->filters().IsEmpty()) {
      exception_state.ThrowTypeError(
          "'filters' member must be non-empty to find any devices.");
      return;
    }

    result->filters.emplace();

    for (const BluetoothLEScanFilterInit* filter : options->filters()) {
      auto canonicalized_filter = mojom::blink::WebBluetoothLeScanFilter::New();

      CanonicalizeFilter(filter, canonicalized_filter, exception_state);

      if (exception_state.HadException())
        return;

      result->filters->push_back(std::move(canonicalized_filter));
    }
  }
}
