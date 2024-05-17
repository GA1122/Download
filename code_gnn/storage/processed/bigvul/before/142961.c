ScriptPromise HTMLMediaElement::playForBindings(ScriptState* script_state) {
  ScriptPromiseResolver* resolver = ScriptPromiseResolver::Create(script_state);
  ScriptPromise promise = resolver->Promise();
  play_promise_resolvers_.push_back(resolver);

  base::Optional<DOMExceptionCode> code = Play();
  if (code) {
    DCHECK(!play_promise_resolvers_.IsEmpty());
    play_promise_resolvers_.pop_back();

    String message;
    switch (code.value()) {
      case DOMExceptionCode::kNotAllowedError:
        message = autoplay_policy_->GetPlayErrorMessage();
        RecordPlayPromiseRejected(
            PlayPromiseRejectReason::kFailedAutoplayPolicy);
        break;
      case DOMExceptionCode::kNotSupportedError:
        message = "The element has no supported sources.";
        RecordPlayPromiseRejected(PlayPromiseRejectReason::kNoSupportedSources);
        break;
      default:
        NOTREACHED();
    }
    resolver->Reject(DOMException::Create(code.value(), message));
    return promise;
  }

  return promise;
}
