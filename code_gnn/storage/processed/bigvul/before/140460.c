bool GetHeaders(base::DictionaryValue* params, std::string* headers) {
  if (!params)
    return false;
  base::ListValue* header_list;
  if (!params->GetList("headers", &header_list))
    return false;
  std::string double_quote_headers;
  base::JSONWriter::Write(header_list, &double_quote_headers);
  base::ReplaceChars(double_quote_headers, "\"", "'", headers);
  return true;
}
