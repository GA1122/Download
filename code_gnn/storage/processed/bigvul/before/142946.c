TextTrack* HTMLMediaElement::addTextTrack(const AtomicString& kind,
                                          const AtomicString& label,
                                          const AtomicString& language,
                                          ExceptionState& exception_state) {


  TextTrack* text_track = TextTrack::Create(kind, label, language);
  text_track->SetReadinessState(TextTrack::kLoaded);

  textTracks()->Append(text_track);


  text_track->setMode(TextTrack::HiddenKeyword());

  return text_track;
}
