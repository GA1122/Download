void TrayCast::OnCastingSessionStartedOrStopped(bool started) {
  is_casting_ = started;
  UpdatePrimaryView();
}
