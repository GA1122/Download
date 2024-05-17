int GetReceivedMessages(FrameTreeNode* ftn) {
  return EvalJs(ftn, "window.receivedMessages;").ExtractInt();
}
