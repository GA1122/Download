void PresentationConnection::setBinaryType(const String& binaryType) {
  if (binaryType == "blob") {
    m_binaryType = BinaryTypeBlob;
    return;
  }
  if (binaryType == "arraybuffer") {
    m_binaryType = BinaryTypeArrayBuffer;
    return;
  }
  ASSERT_NOT_REACHED();
}
