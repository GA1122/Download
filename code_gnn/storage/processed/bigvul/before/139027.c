KURL ImageLoader::imageSourceToKURL(AtomicString imageSourceURL) const {
  KURL url;

  Document& document = m_element->document();
  if (!document.isActive())
    return url;

  if (!imageSourceURL.isNull()) {
    String strippedImageSourceURL =
        stripLeadingAndTrailingHTMLSpaces(imageSourceURL);
    if (!strippedImageSourceURL.isEmpty())
      url = document.completeURL(strippedImageSourceURL);
  }
  return url;
}
