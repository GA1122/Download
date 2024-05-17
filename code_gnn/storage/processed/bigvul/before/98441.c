void ResizeGripper::ReportResizeAmount(int resize_amount, bool last_update) {
  gfx::Point point(resize_amount, 0);
  View::ConvertPointToScreen(this, &point);
  resize_amount = point.x() - initial_position_;

  if (base::i18n::IsRTL())
    resize_amount = -1 * resize_amount;
   delegate_->OnResize(resize_amount, last_update);
 }
