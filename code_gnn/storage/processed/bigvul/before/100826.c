std::string HttpUtil::ConvertHeadersBackToHTTPResponse(const std::string& str) {
  std::string disassembled_headers;
  StringTokenizer tokenizer(str, std::string(1, '\0'));
  while (tokenizer.GetNext()) {
    disassembled_headers.append(tokenizer.token_begin(), tokenizer.token_end());
    disassembled_headers.append("\r\n");
  }
  disassembled_headers.append("\r\n");

  return disassembled_headers;
}
