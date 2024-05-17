SpeechRecognitionManagerImpl::GetDelegateListener() const {
  return delegate_.get() ? delegate_->GetEventListener() : nullptr;
}
