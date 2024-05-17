void PDFiumEngine::ScrolledToXPosition(int position) {
  CancelPaints();

  int old_x = position_.x();
  position_.set_x(position);
  CalculateVisiblePages();
  client_->Scroll(pp::Point(old_x - position, 0));
}
