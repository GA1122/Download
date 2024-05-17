const AtomicString& Document::RequiredCSP() {
  if (!Loader())
    return g_null_atom;
  return frame_->Loader().RequiredCSP();
}
