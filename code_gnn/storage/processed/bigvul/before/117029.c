size_t OmniboxPopupViewGtk::LineFromY(int y) {
  size_t line = std::max(y - kBorderThickness, 0) / kHeightPerResult;
  return std::min(line, model_->result().size() - 1);
}
