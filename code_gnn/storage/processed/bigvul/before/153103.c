void PDFiumEngine::RotateCounterclockwise() {
  current_rotation_ = (current_rotation_ - 1) % 4;
  RotateInternal();
}
