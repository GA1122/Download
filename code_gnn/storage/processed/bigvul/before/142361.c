void PrintTo(const UserInfo::Field& field, std::ostream* os) {
  *os << "(display text: \"" << base::UTF16ToUTF8(field.display_text())
      << "\", a11y_description: \""
      << base::UTF16ToUTF8(field.a11y_description()) << "\", is "
      << (field.is_obfuscated() ? "" : "not ") << "obfuscated, is "
      << (field.selectable() ? "" : "not ") << "selectable)";
}
