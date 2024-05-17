static bool IsDangerousHTTPEquiv(const String& value) {
  String equiv = value.StripWhiteSpace();
  return DeprecatedEqualIgnoringCase(equiv, "refresh") ||
         DeprecatedEqualIgnoringCase(equiv, "set-cookie");
}
