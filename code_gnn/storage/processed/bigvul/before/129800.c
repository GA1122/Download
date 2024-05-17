String NonNullString(const String& string) {
  return string.IsNull() ? g_empty_string16_bit : string;
}
