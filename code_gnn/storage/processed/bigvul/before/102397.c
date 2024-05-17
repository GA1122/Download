std::string GetFileNameFromCD(const std::string& header,
                              const std::string& referrer_charset) {
  std::string decoded;
  std::string param_value = GetHeaderParamValue(header, "filename*",
                                                QuoteRule::KEEP_OUTER_QUOTES);
  if (!param_value.empty()) {
    if (param_value.find('"') == std::string::npos) {
      std::string charset;
      std::string value;
      if (DecodeCharset(param_value, &charset, &value)) {
        if (!IsStringASCII(value))
          return std::string();
        std::string tmp = UnescapeURLComponent(
            value,
            UnescapeRule::SPACES | UnescapeRule::URL_SPECIAL_CHARS);
        if (base::ConvertToUtf8AndNormalize(tmp, charset, &decoded))
          return decoded;
      }
    }
  }
  param_value = GetHeaderParamValue(header, "filename",
                                    QuoteRule::REMOVE_OUTER_QUOTES);
  if (param_value.empty()) {
    param_value = GetHeaderParamValue(header, "name",
                                      QuoteRule::REMOVE_OUTER_QUOTES);
  }
  if (param_value.empty())
    return std::string();
  if (DecodeParamValue(param_value, referrer_charset, &decoded))
    return decoded;
  return std::string();
}
