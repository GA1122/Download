bool Plugin::NexeIsContentHandler() const {
  return
      !mime_type().empty() &&
      mime_type() != kNaClMIMEType;
}
