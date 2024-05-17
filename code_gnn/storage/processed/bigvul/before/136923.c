static bool IsValidFileExtension(const String& type) {
  if (type.length() < 2)
    return false;
  return type[0] == '.';
}
