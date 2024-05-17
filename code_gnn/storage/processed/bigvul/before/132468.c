void UsbGetDevicesFunction::OnGetDevicesComplete(
    const std::vector<scoped_refptr<UsbDevice>>& devices) {
  scoped_ptr<base::ListValue> result(new base::ListValue());
  UsbGuidMap* guid_map = UsbGuidMap::Get(browser_context());
  for (const scoped_refptr<UsbDevice>& device : devices) {
    if ((filters_.empty() || UsbDeviceFilter::MatchesAny(device, filters_)) &&
        HasDevicePermission(device)) {
      result->Append(PopulateDevice(device.get(),
                                    guid_map->GetIdFromGuid(device->guid())));
    }
  }

  Respond(OneArgument(result.release()));
}
