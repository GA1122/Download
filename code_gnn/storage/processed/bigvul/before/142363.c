void PrintTo(const FooterCommand& footer_command, std::ostream* os) {
  *os << "(display text: \"" << base::UTF16ToUTF8(footer_command.display_text())
      << "\")";
}
