bool Editor::Command::IsSupported() const {
  if (!command_)
    return false;
  switch (source_) {
    case kCommandFromMenuOrKeyBinding:
      return true;
    case kCommandFromDOM:
      return command_->is_supported_from_dom(frame_.Get());
  }
  NOTREACHED();
  return false;
}
