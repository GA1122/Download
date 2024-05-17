static bool StartsOpeningScriptTagAt(const String& string, size_t start) {
  if (start + 6 >= string.length())
    return false;
  StringView script("<script");
  return EqualIgnoringASCIICase(StringView(string, start, script.length()),
                                script);
}
