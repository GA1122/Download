static bool LookingAtNAL(const std::string& encoded, size_t pos) {
  return pos + 3 < encoded.size() &&
      encoded[pos] == 0 && encoded[pos + 1] == 0 &&
      encoded[pos + 2] == 0 && encoded[pos + 3] == 1;
}
