String HTMLMediaElement::canPlayType(const String& mimeType) const {
  MIMETypeRegistry::SupportsType support = supportsType(ContentType(mimeType));
  String canPlay;

  switch (support) {
    case MIMETypeRegistry::IsNotSupported:
      canPlay = emptyString;
      break;
    case MIMETypeRegistry::MayBeSupported:
      canPlay = "maybe";
      break;
    case MIMETypeRegistry::IsSupported:
      canPlay = "probably";
      break;
  }

  BLINK_MEDIA_LOG << "canPlayType(" << (void*)this << ", " << mimeType
                  << ") -> " << canPlay;

  return canPlay;
}
