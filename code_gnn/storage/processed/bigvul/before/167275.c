void FetchManager::Loader::Trace(blink::Visitor* visitor) {
  visitor->Trace(fetch_manager_);
  visitor->Trace(resolver_);
  visitor->Trace(fetch_request_data_);
  visitor->Trace(threadable_loader_);
  visitor->Trace(integrity_verifier_);
  visitor->Trace(signal_);
  visitor->Trace(execution_context_);
}
