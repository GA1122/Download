int PDFiumEngine::Form_GetFilePath(IPDF_JSPLATFORM* param,
                                   void* file_path,
                                   int length) {
  PDFiumEngine* engine = static_cast<PDFiumEngine*>(param);
  std::string rv = engine->client_->GetURL();
  if (file_path && rv.size() <= static_cast<size_t>(length))
    memcpy(file_path, rv.c_str(), rv.size());
  return rv.size();
}
