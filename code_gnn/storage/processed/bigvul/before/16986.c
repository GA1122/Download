bool WebContext::doNotTrack() const {
    if (IsInitialized()) {
    return context_->GetDoNotTrack();
  }

  return construct_props_->do_not_track;
}
