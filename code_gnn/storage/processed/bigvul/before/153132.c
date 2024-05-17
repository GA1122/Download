void PDFiumEngine::UpdateTickMarks() {
  std::vector<pp::Rect> tickmarks;
  GetAllScreenRectsUnion(&find_results_, pp::Point(0, 0), &tickmarks);
  client_->UpdateTickMarks(tickmarks);
}
