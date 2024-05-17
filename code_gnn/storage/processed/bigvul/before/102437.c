bool ParseCIDRBlock(const std::string& cidr_literal,
                    IPAddressNumber* ip_number,
                    size_t* prefix_length_in_bits) {

  std::vector<std::string> parts;
  base::SplitString(cidr_literal, '/', &parts);
  if (parts.size() != 2)
    return false;

  if (!ParseIPLiteralToNumber(parts[0], ip_number))
    return false;

  int number_of_bits = -1;
  if (!base::StringToInt(parts[1], &number_of_bits))
    return false;

  if (number_of_bits < 0 ||
      number_of_bits > static_cast<int>(ip_number->size() * 8))
    return false;

  *prefix_length_in_bits = static_cast<size_t>(number_of_bits);
  return true;
}
