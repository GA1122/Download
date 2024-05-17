  std::string GetAlternateProtocolHttpHeader() {
    return std::string("Alternate-Protocol: 443:") +
        GetAlternateProtocolFromParam() + "\r\n\r\n";
  }
