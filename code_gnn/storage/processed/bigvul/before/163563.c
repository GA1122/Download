XmlReader::~XmlReader() {
  if (reader_)
    xmlFreeTextReader(reader_);
}
