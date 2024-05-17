void Document::SetBodyAttribute(const QualifiedName& name,
                                const AtomicString& value) {
  if (auto* bodyElement = body()) {
    if (bodyElement->FastGetAttribute(name) != value)
      bodyElement->setAttribute(name, value);
  }
}
