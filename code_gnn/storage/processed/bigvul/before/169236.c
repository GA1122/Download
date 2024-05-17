bool FrameMatchesName(const std::string& name, RenderFrameHost* frame) {
  return frame->GetFrameName() == name;
}
