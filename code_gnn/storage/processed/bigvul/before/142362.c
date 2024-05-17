void PrintTo(const UserInfo& user_info, std::ostream* os) {
  *os << "[";
  for (const UserInfo::Field& field : user_info.fields()) {
    PrintTo(field, os);
    *os << ", ";
  }
  *os << "]";
}
