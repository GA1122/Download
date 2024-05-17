ChromeContentBrowserClient::GetTtsControllerDelegate() {
  TtsControllerDelegateImpl* delegate =
      TtsControllerDelegateImpl::GetInstance();
#if !defined(OS_ANDROID)
  TtsExtensionEngine* tts_extension_engine = TtsExtensionEngine::GetInstance();
  delegate->SetTtsEngineDelegate(tts_extension_engine);
#endif
  return delegate;
}
