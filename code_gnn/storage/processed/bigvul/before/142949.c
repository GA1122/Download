String HTMLMediaElement::canPlayType(const String& mime_type) const {
  MIMETypeRegistry::SupportsType support =
      GetSupportsType(ContentType(mime_type));
  String can_play;

  switch (support) {
    case MIMETypeRegistry::kIsNotSupported:
      can_play = g_empty_string;
      break;
    case MIMETypeRegistry::kMayBeSupported:
      can_play = "maybe";
      break;
    case MIMETypeRegistry::kIsSupported:
      can_play = "probably";
      break;
  }

  BLINK_MEDIA_LOG << "canPlayType(" << (void*)this << ", " << mime_type
                  << ") -> " << can_play;

  return can_play;
}
