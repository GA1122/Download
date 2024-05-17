int Editor::Command::IdForHistogram() const {
  return IsSupported() ? static_cast<int>(command_->command_type) : 0;
}
