bool ParseIPLiteralToNumber(const std::string& ip_literal,
                            IPAddressNumber* ip_number) {
  if (ip_literal.find(':') != std::string::npos) {
    std::string host_brackets = "[" + ip_literal + "]";
    url_parse::Component host_comp(0, host_brackets.size());

    ip_number->resize(16);   
    return url_canon::IPv6AddressToNumber(host_brackets.data(),
                                          host_comp,
                                          &(*ip_number)[0]);
  }

  ip_number->resize(4);   
  url_parse::Component host_comp(0, ip_literal.size());
  int num_components;
  url_canon::CanonHostInfo::Family family = url_canon::IPv4AddressToNumber(
      ip_literal.data(), host_comp, &(*ip_number)[0], &num_components);
  return family == url_canon::CanonHostInfo::IPV4;
}
