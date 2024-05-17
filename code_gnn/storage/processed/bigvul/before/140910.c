void PresentationConnection::didReceiveBinaryMessage(const uint8_t* data,
                                                     size_t length) {
  if (m_state != WebPresentationConnectionState::Connected)
    return;

  switch (m_binaryType) {
    case BinaryTypeBlob: {
      std::unique_ptr<BlobData> blobData = BlobData::create();
      blobData->appendBytes(data, length);
      Blob* blob =
          Blob::create(BlobDataHandle::create(std::move(blobData), length));
      dispatchEvent(MessageEvent::create(blob));
      return;
    }
    case BinaryTypeArrayBuffer:
      DOMArrayBuffer* buffer = DOMArrayBuffer::create(data, length);
      dispatchEvent(MessageEvent::create(buffer));
      return;
  }
  ASSERT_NOT_REACHED();
}
