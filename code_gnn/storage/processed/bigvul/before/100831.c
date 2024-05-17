std::string HttpUtil::GenerateAcceptCharsetHeader(const std::string& charset) {
  std::string charset_with_q = charset;
  if (LowerCaseEqualsASCII(charset, "utf-8")) {
    charset_with_q += ",*;q=0.5";
  } else {
    charset_with_q += ",utf-8;q=0.7,*;q=0.3";
  }
  return charset_with_q;
}
