std::string SessionModelAssociator::GetComputerName() {
  char computer_name[MAX_COMPUTERNAME_LENGTH + 1];
  DWORD size = sizeof(computer_name);
  if (GetComputerNameA(computer_name, &size)) {
    return computer_name;
  }
  return std::string();
}
