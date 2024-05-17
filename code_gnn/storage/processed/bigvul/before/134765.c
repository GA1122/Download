void EventConverterEvdevImpl::AllowAllKeys() {
  DCHECK(HasKeyboard());
  blocked_keys_.reset();
}
