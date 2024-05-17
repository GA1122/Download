void ReadableStreamBytesConsumer::Trace(blink::Visitor* visitor) {
  visitor->Trace(client_);
  visitor->Trace(pending_buffer_);
  BytesConsumer::Trace(visitor);
}
