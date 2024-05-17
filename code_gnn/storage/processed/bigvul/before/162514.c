void ClassicPendingScript::Trace(blink::Visitor* visitor) {
  visitor->Trace(streamer_);
  ResourceClient::Trace(visitor);
  MemoryCoordinatorClient::Trace(visitor);
  PendingScript::Trace(visitor);
}
