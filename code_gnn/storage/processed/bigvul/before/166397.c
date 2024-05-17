std::string GLES2Util::GetStringEnum(uint32_t value) {
  const EnumToString* entry = enum_to_string_table_;
  const EnumToString* end = entry + enum_to_string_table_len_;
  for (; entry < end; ++entry) {
    if (value == entry->value)
      return entry->name;
  }
  std::stringstream ss;
  ss.fill('0');
  ss.width(value < 0x10000 ? 4 : 8);
  ss << std::hex << value;
  return "0x" + ss.str();
}
