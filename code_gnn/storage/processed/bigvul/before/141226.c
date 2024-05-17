mojo::ScopedMessagePipeHandle Document::SetDocumentInterfaceBrokerForTesting(
    mojo::ScopedMessagePipeHandle blink_handle) {
  DCHECK(GetFrame());
  return GetFrame()->SetDocumentInterfaceBrokerForTesting(
      std::move(blink_handle));
}
