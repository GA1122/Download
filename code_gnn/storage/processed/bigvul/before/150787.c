bool IsEmptyOrInvalidFilter(
    const blink::mojom::WebBluetoothLeScanFilterPtr& filter) {
  if (!filter->name && !filter->name_prefix && !filter->services)
    return true;

  if (filter->name && filter->name->size() > kMaxLengthForDeviceName)
    return true;

  if (filter->name_prefix &&
      filter->name_prefix->size() > kMaxLengthForDeviceName)
    return true;

  if (filter->name_prefix && filter->name_prefix->empty())
    return true;

  return false;
}
