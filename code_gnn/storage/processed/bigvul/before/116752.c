void MockRenderThread::VerifyRunJavaScriptMessageSend(
    const string16& expected_alert_message) {
  const IPC::Message* alert_msg =
      sink_.GetUniqueMessageMatching(ViewHostMsg_RunJavaScriptMessage::ID);
  ASSERT_TRUE(alert_msg);
  PickleIterator iter = IPC::SyncMessage::GetDataIterator(alert_msg);
  ViewHostMsg_RunJavaScriptMessage::SendParam alert_param;
  ASSERT_TRUE(IPC::ReadParam(alert_msg, &iter, &alert_param));
  EXPECT_EQ(expected_alert_message, alert_param.a);
}
