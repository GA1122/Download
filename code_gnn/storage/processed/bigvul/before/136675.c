void FrameLoader::RecordLatestRequiredCSP() {
  required_csp_ = frame_->Owner() ? frame_->Owner()->Csp() : g_null_atom;
}
