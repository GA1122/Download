void WebBluetoothServiceImpl::StoreAllowedScanOptions(
    const blink::mojom::WebBluetoothRequestLEScanOptions& options) {
  if (options.filters.has_value()) {
    for (const auto& filter : options.filters.value())
      allowed_scan_filters_.push_back(filter.Clone());
  } else {
    accept_all_advertisements_ = true;
  }
}
