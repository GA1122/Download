bool MatchesFilters(
    const std::string* device_name,
    const UUIDSet& device_uuids,
    const base::Optional<
        std::vector<blink::mojom::WebBluetoothLeScanFilterPtr>>& filters) {
  DCHECK(!HasEmptyOrInvalidFilter(filters));
  for (const auto& filter : filters.value()) {
    if (MatchesFilter(device_name, device_uuids, filter)) {
      return true;
    }
  }
  return false;
}
