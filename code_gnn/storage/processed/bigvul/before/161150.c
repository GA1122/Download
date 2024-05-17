std::string RandomLabel() {
  static const char kAlphabet[] = "0123456789"
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

  static const size_t kRfc4122LengthLabel = 36u;
  std::string label(kRfc4122LengthLabel, ' ');
  for (char& c : label) {
    c = kAlphabet[base::RandGenerator(arraysize(kAlphabet) - 1)];
    DCHECK(std::isalnum(c)) << c;
  }
  return label;
}
