bool PrintWebViewHelper::PrintPreviewContext::IsModifiable() const {
  if (node())
    return false;
  std::string mime(frame()->dataSource()->response().mimeType().utf8());
  return mime != "application/pdf";
}
