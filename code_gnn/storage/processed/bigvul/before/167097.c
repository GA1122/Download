bool IsValidPsm(int psm) {
  if (psm <= 0)
    return false;

  std::vector<int16_t> octets;
  while (psm > 0) {
     octets.push_back(psm & 0xFF);
     psm = psm >> 8;
  }

  if (octets.size() < 2U)
    return false;

  if ((octets.front() & 0x01) != 1)
    return false;

  if ((octets.back() & 0x01) != 0)
    return false;

  return true;
}
