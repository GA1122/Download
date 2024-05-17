std::string PrettyPrintEscapedJson(const std::string& query) {
  const std::string json = net::UnescapeURLComponent(
      query, net::UnescapeRule::SPACES | net::UnescapeRule::URL_SPECIAL_CHARS);
  scoped_ptr<base::Value> value(base::JSONReader::Read(json));
  std::string pretty_json;
  base::JSONWriter::WriteWithOptions(value.get(),
                                     base::JSONWriter::OPTIONS_PRETTY_PRINT,
                                     &pretty_json);
   return pretty_json;
 }
