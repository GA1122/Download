void HTMLMediaElement::RejectPlayPromises(DOMExceptionCode code,
                                          const String& message) {
  play_promise_reject_list_.AppendVector(play_promise_resolvers_);
  play_promise_resolvers_.clear();
  RejectPlayPromisesInternal(code, message);
}
