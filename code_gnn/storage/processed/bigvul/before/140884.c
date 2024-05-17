PresentationConnectionProxy::PresentationConnectionProxy(
    blink::WebPresentationConnection* source_connection)
    : binding_(this),
      target_connection_ptr_(nullptr),
      source_connection_(source_connection) {
  DCHECK(source_connection_);
}
