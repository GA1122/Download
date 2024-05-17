  base::FilePath GetComputerPath(const std::string& computer_name) {
    return GetComputerGrandRoot().Append(computer_name);
  }
