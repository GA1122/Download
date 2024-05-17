void HTMLMediaElement::RejectPlayPromisesInternal(DOMExceptionCode code,
                                                  const String& message) {
  DCHECK(code == DOMExceptionCode::kAbortError ||
         code == DOMExceptionCode::kNotSupportedError);

  for (auto& resolver : play_promise_reject_list_)
    resolver->Reject(DOMException::Create(code, message));

  play_promise_reject_list_.clear();
}
