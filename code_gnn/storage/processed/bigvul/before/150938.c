static void CanonicalizeFilter(
    const BluetoothLEScanFilterInit* filter,
    mojom::blink::WebBluetoothLeScanFilterPtr& canonicalized_filter,
    ExceptionState& exception_state) {
  if (!(filter->hasServices() || filter->hasName() ||
        filter->hasNamePrefix())) {
    exception_state.ThrowTypeError(
        "A filter must restrict the devices in some way.");
    return;
  }

  if (filter->hasServices()) {
    if (filter->services().size() == 0) {
      exception_state.ThrowTypeError(
          "'services', if present, must contain at least one service.");
      return;
    }
    canonicalized_filter->services.emplace();
    for (const StringOrUnsignedLong& service : filter->services()) {
      const String& validated_service =
          BluetoothUUID::getService(service, exception_state);
      if (exception_state.HadException())
        return;
      canonicalized_filter->services->push_back(validated_service);
    }
  }

  if (filter->hasName()) {
    size_t name_length = filter->name().Utf8().length();
    if (name_length > kMaxDeviceNameLength) {
      exception_state.ThrowTypeError(kDeviceNameTooLong);
      return;
    }
    canonicalized_filter->name = filter->name();
  }

  if (filter->hasNamePrefix()) {
    size_t name_prefix_length = filter->namePrefix().Utf8().length();
    if (name_prefix_length > kMaxDeviceNameLength) {
      exception_state.ThrowTypeError(kDeviceNameTooLong);
      return;
    }
    if (filter->namePrefix().length() == 0) {
      exception_state.ThrowTypeError(
          "'namePrefix', if present, must me non-empty.");
      return;
    }
    canonicalized_filter->name_prefix = filter->namePrefix();
  }
}
