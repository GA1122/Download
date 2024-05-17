void FrameLoader::RecordLatestRequiredCSP() {
  required_csp_ =
      frame_->Owner() ? frame_->Owner()->RequiredCsp() : g_null_atom;
}
