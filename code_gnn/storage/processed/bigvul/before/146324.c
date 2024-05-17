bool IsPrefixReserved(const String& name) {
  if (name.StartsWith("gl_") || name.StartsWith("webgl_") ||
      name.StartsWith("_webgl_"))
    return true;
  return false;
}
