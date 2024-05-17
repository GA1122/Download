bool HasEmptyOrInvalidFilter(
    const base::Optional<
        std::vector<blink::mojom::WebBluetoothLeScanFilterPtr>>& filters) {
  if (!filters) {
    return true;
  }

  return filters->empty()
             ? true
             : filters->end() != std::find_if(filters->begin(), filters->end(),
                                              IsEmptyOrInvalidFilter);
}
