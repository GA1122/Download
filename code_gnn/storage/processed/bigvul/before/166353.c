void SpeechRecognitionManagerImpl::SessionStopAudioCapture(
    const Session& session) {
  DCHECK(session.recognizer.get());
  session.recognizer->StopAudioCapture();
}
