InputEventAckState InputMsgWatcher::GetAckStateWaitIfNecessary() {
  if (HasReceivedAck())
    return ack_result_;
  return WaitForAck();
}
