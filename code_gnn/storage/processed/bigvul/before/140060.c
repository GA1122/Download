void HTMLMediaElement::rejectPlayPromisesInternal(ExceptionCode code,
                                                  const String& message) {
  DCHECK(code == AbortError || code == NotSupportedError);

  for (auto& resolver : m_playPromiseRejectList)
    resolver->reject(DOMException::create(code, message));

  m_playPromiseRejectList.clear();
}
