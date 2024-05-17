    ChromeContentBrowserClient::GetSpeechRecognitionManagerDelegate() {
#if defined(ENABLE_INPUT_SPEECH)
  return new speech::ChromeSpeechRecognitionManagerDelegate();
#else
  return NULL;
#endif
}
