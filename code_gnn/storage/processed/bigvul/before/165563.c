void ContentSecurityPolicy::Trace(blink::Visitor* visitor) {
  visitor->Trace(execution_context_);
  visitor->Trace(policies_);
  visitor->Trace(console_messages_);
  visitor->Trace(self_source_);
}
