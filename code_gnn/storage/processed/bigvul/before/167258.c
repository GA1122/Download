void FetchManager::Loader::Failed(const String& message) {
  if (failed_ || finished_)
    return;
  failed_ = true;
  if (execution_context_->IsContextDestroyed())
    return;
  if (!message.IsEmpty()) {
    execution_context_->AddConsoleMessage(
        ConsoleMessage::Create(kJSMessageSource, kErrorMessageLevel, message));
  }
  if (resolver_) {
    ScriptState* state = resolver_->GetScriptState();
    ScriptState::Scope scope(state);
    resolver_->Reject(V8ThrowException::CreateTypeError(state->GetIsolate(),
                                                        "Failed to fetch"));
  }
  probe::didFailFetch(execution_context_, this);
  NotifyFinished();
}
