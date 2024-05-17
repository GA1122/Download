static void ConvertHexadecimalToIDAlphabet(std::string* id) {
  for (size_t i = 0; i < id->size(); ++i) {
    int val;
    if (base::HexStringToInt(id->begin() + i, id->begin() + i + 1, &val))
      (*id)[i] = val + 'a';
    else
      (*id)[i] = 'a';
  }
}
