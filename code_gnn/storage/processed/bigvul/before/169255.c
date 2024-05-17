bool InputMsgWatcher::HasReceivedAck() const {
  return ack_result_ != INPUT_EVENT_ACK_STATE_UNKNOWN;
}
