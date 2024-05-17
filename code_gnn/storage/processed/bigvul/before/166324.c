SpeechRecognitionManager* SpeechRecognitionManager::GetInstance() {
  if (manager_for_tests_)
    return manager_for_tests_;
  return SpeechRecognitionManagerImpl::GetInstance();
}
