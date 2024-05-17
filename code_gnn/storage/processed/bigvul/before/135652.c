void FrameSelection::NodeChildrenWillBeRemoved(ContainerNode& container) {
  if (!container.InActiveDocument())
    return;
  if (!GetDocument().IsRunningExecCommand())
    TypingCommand::CloseTyping(frame_);
}
