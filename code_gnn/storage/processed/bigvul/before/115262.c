void OmniboxViewWin::OnRButtonDown(UINT  , const CPoint& point) {
  TrackMousePosition(kRight, point);
  tracking_double_click_ = false;
  possible_drag_ = false;
  gaining_focus_.reset();
  SetMsgHandled(false);
}
