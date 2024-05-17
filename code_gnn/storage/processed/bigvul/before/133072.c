void HWNDMessageHandler::DidProcessEvent(const base::NativeEvent& event) {
  RedrawInvalidRect();
}
