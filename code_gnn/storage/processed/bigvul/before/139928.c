TextTrack* HTMLMediaElement::addTextTrack(const AtomicString& kind,
                                          const AtomicString& label,
                                          const AtomicString& language,
                                          ExceptionState& exceptionState) {


  TextTrack* textTrack = TextTrack::create(kind, label, language);
  textTrack->setReadinessState(TextTrack::Loaded);

  textTracks()->append(textTrack);


  textTrack->setMode(TextTrack::hiddenKeyword());

  return textTrack;
}
