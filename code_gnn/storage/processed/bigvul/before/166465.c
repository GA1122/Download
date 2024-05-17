  void GetExtendedAttributeNames(vector<string>* attr_names) const {
    ssize_t len = listxattr(test_file().value().c_str(), nullptr, 0);
    if (len <= static_cast<ssize_t>(0))
      return;
    char* buffer = new char[len];
    len = listxattr(test_file().value().c_str(), buffer, len);
    *attr_names =
        base::SplitString(string(buffer, len), std::string(1, '\0'),
                          base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
    delete[] buffer;
  }
