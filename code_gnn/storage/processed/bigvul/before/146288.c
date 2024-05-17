String WebGLRenderingContextBase::EnsureNotNull(const String& text) const {
  if (text.IsNull())
    return WTF::g_empty_string;
  return text;
}
