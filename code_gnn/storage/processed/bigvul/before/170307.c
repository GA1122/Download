std::string UsbChooserContext::GetObjectName(
    const base::DictionaryValue& object) {
  DCHECK(IsValidObject(object));
  std::string name;
  bool found = object.GetString(kDeviceNameKey, &name);
  DCHECK(found);
  return name;
}
