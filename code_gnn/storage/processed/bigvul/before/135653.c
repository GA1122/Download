void FrameSelection::NodeWillBeRemoved(Node& node) {
  if (!node.InActiveDocument())
    return;
  if (!GetDocument().IsRunningExecCommand())
    TypingCommand::CloseTyping(frame_);
}
