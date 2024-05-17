base::string16 password_for_str(const std::string& user) {
  return password_for_str(ASCIIToUTF16(user));
}
