void UsbGetUserSelectedDevicesFunction::OnDevicesChosen(
    const std::vector<scoped_refptr<UsbDevice>>& devices) {
  scoped_ptr<base::ListValue> result(new base::ListValue());
  UsbGuidMap* guid_map = UsbGuidMap::Get(browser_context());
  for (const auto& device : devices) {
    result->Append(
        PopulateDevice(device.get(), guid_map->GetIdFromGuid(device->guid())));
  }

  Respond(OneArgument(result.release()));
}
