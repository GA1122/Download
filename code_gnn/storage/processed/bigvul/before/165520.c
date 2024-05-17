Document* ContentSecurityPolicy::GetDocument() const {
  return DynamicTo<Document>(execution_context_.Get());
}
