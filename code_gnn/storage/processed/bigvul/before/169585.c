bool HexDecode(const std::string& input, std::string* output) {
  std::vector<uint8_t> bytes;
  if (!base::HexStringToBytes(input, &bytes))
    return false;
  output->assign(reinterpret_cast<const char*>(&bytes[0]), bytes.size());
  return true;
}
