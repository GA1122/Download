bool Editor::Command::Execute(const String& parameter,
                              Event* triggering_event) const {
  if (!IsEnabled(triggering_event)) {
    if (!IsSupported() || !frame_ || !command_->allow_execution_when_disabled)
      return false;
  }

  if (source_ == kCommandFromMenuOrKeyBinding) {
    InputEvent::InputType input_type =
        InputTypeFromCommandType(command_->command_type, *frame_);
    if (input_type != InputEvent::InputType::kNone) {
      if (DispatchBeforeInputEditorCommand(
              EventTargetNodeForDocument(frame_->GetDocument()), input_type,
              GetTargetRanges()) != DispatchEventResult::kNotCanceled)
        return true;
    }
  }

  if (frame_->GetDocument()->GetFrame() != frame_)
    return false;

  GetFrame().GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();
  DEFINE_STATIC_LOCAL(SparseHistogram, command_histogram,
                      ("WebCore.Editing.Commands"));
  command_histogram.Sample(static_cast<int>(command_->command_type));
  return command_->execute(*frame_, triggering_event, source_, parameter);
}
