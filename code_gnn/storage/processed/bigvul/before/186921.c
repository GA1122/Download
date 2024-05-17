  bool XmlReader::Load(const std::string& input) {
    const int kParseOptions = XML_PARSE_RECOVER |   
                            XML_PARSE_NONET |     
                            XML_PARSE_NOXXE;      
//                              XML_PARSE_NONET;     
    reader_ = xmlReaderForMemory(input.data(), static_cast<int>(input.size()),
                                NULL, NULL, kParseOptions);
   return reader_ != NULL;
 }