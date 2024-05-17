bool AudioHandler::InputsAreSilent() {
  for (auto& input : inputs_) {
    if (!input->Bus()->IsSilent())
      return false;
  }
  return true;
}
