std::string XmlStringToStdString(const xmlChar* xmlstring) {
  if (xmlstring)
    return std::string(reinterpret_cast<const char*>(xmlstring));
  else
    return "";
}
