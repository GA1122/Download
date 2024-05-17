  virtual void visitHeader(const WebString& name, const WebString& value) {
    if (!name.isEmpty() && !value.isEmpty()) {
      buf_->append(name.utf8());
      buf_->append(": ");
      buf_->append(value.utf8());
      buf_->append("\n");
    }
  }
