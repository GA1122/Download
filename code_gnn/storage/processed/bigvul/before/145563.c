CtapDeviceResponseCode GetResponseCode(base::span<const uint8_t> buffer) {
  if (buffer.empty())
    return CtapDeviceResponseCode::kCtap2ErrInvalidCBOR;

  auto code = static_cast<CtapDeviceResponseCode>(buffer[0]);
  return base::ContainsValue(GetCtapResponseCodeList(), code)
             ? code
             : CtapDeviceResponseCode::kCtap2ErrInvalidCBOR;
}
