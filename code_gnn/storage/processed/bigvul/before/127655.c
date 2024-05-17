XScopedString::~XScopedString() {
  XFree(string_);
}
