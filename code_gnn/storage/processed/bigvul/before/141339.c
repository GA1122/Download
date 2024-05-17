Node* Document::importNode(Node* imported_node,
                           bool deep,
                           ExceptionState& exception_state) {

  if (imported_node->IsDocumentNode()) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kNotSupportedError,
        "The node provided is a document, which may not be imported.");
    return nullptr;
  }
  if (imported_node->IsShadowRoot()) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kNotSupportedError,
        "The node provided is a shadow root, which may not be imported.");
    return nullptr;
  }

  return imported_node->Clone(
      *this, deep ? CloneChildrenFlag::kClone : CloneChildrenFlag::kSkip);
}
