ScriptPromise HTMLMediaElement::playForBindings(ScriptState* scriptState) {
  ScriptPromiseResolver* resolver = ScriptPromiseResolver::create(scriptState);
  ScriptPromise promise = resolver->promise();
  m_playPromiseResolvers.push_back(resolver);

  Nullable<ExceptionCode> code = play();
  if (!code.isNull()) {
    DCHECK(!m_playPromiseResolvers.isEmpty());
    m_playPromiseResolvers.pop_back();

    String message;
    switch (code.get()) {
      case NotAllowedError:
        message = "play() can only be initiated by a user gesture.";
        break;
      case NotSupportedError:
        message = "The element has no supported sources.";
        break;
      default:
        NOTREACHED();
    }
    resolver->reject(DOMException::create(code.get(), message));
    return promise;
  }

  return promise;
}
