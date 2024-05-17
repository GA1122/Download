bool SplitStringIntoKeyValues(
    const std::string& line,
    char key_value_delimiter,
    std::string* key, std::vector<std::string>* values) {
  key->clear();
  values->clear();

  size_t end_key_pos = line.find_first_of(key_value_delimiter);
  if (end_key_pos == std::string::npos) {
    DVLOG(1) << "cannot parse key from line: " << line;
    return false;     
  }
  key->assign(line, 0, end_key_pos);

  std::string remains(line, end_key_pos, line.size() - end_key_pos);
  size_t begin_values_pos = remains.find_first_not_of(key_value_delimiter);
  if (begin_values_pos == std::string::npos) {
    DVLOG(1) << "cannot parse value from line: " << line;
    return false;    
  }
  std::string values_string(remains, begin_values_pos,
                            remains.size() - begin_values_pos);

  values->push_back(values_string);
  return true;
}
