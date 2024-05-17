void WebContext::setDoNotTrack(bool dnt) {
  if (IsInitialized()) {
    context_->SetDoNotTrack(dnt);
  } else {
    construct_props_->do_not_track = dnt;
  }
}
