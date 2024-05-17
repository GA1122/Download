void FrameFetchContext::Trace(blink::Visitor* visitor) {
  visitor->Trace(document_loader_);
  visitor->Trace(document_);
  visitor->Trace(frozen_state_);
  visitor->Trace(fetch_client_settings_object_);
  BaseFetchContext::Trace(visitor);
}
