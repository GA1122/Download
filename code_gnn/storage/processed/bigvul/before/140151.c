HTMLMediaElement::~HTMLMediaElement() {
  BLINK_MEDIA_LOG << "~HTMLMediaElement(" << (void*)this << ")";

  DCHECK(!m_audioSourceNode);
}
