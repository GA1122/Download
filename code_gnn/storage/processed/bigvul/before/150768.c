bool WebBluetoothServiceImpl::AreScanFiltersAllowed(
    const base::Optional<ScanFilters>& filters) const {
  if (accept_all_advertisements_) {
    return true;
  }

  if (!filters.has_value()) {
    return false;
  }

  for (const auto& filter : filters.value()) {
    bool allowed = false;
    for (const auto& allowed_filter : allowed_scan_filters_) {
      if (AreScanFiltersSame(*filter, *allowed_filter)) {
        allowed = true;
        break;
      }
    }

    if (!allowed)
      return false;
  }

  return true;
}
