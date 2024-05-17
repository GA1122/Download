std::string GLES2Util::GetStringError(uint32_t value) {
  static EnumToString string_table[] = {
    { GL_NONE, "GL_NONE" },
  };
  return GLES2Util::GetQualifiedEnumString(
      string_table, arraysize(string_table), value);
}
