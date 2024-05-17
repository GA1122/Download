HTMLMediaElement::~HTMLMediaElement() {
  BLINK_MEDIA_LOG << "~HTMLMediaElement(" << (void*)this << ")";

  DCHECK(!audio_source_node_);
}
