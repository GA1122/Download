  bool XmlReader::LoadFile(const std::string& file_path) {
    const int kParseOptions = XML_PARSE_RECOVER |   
                            XML_PARSE_NONET |     
                            XML_PARSE_NOXXE;      
//                             XML_PARSE_NONET;      
    reader_ = xmlReaderForFile(file_path.c_str(), NULL, kParseOptions);
    return reader_ != NULL;
  }