void ImageLoader::elementDidMoveToNewDocument() {
  if (m_loadDelayCounter)
    m_loadDelayCounter->documentChanged(m_element->document());
  clearFailedLoadURL();
  setImage(0);
}
