bool WebRTCAudioDeviceTest::OnMessageReceived(const IPC::Message& message) {
  if (render_thread_) {
    IPC::ChannelProxy::MessageFilter* filter =
        render_thread_->audio_input_message_filter();
    if (filter->OnMessageReceived(message))
      return true;

    filter = render_thread_->audio_message_filter();
    if (filter->OnMessageReceived(message))
      return true;
  }

  if (audio_render_host_.get()) {
    bool message_was_ok = false;
    if (audio_render_host_->OnMessageReceived(message, &message_was_ok))
      return true;
  }

  if (audio_input_renderer_host_.get()) {
    bool message_was_ok = false;
    if (audio_input_renderer_host_->OnMessageReceived(message, &message_was_ok))
      return true;
  }

  bool handled ALLOW_UNUSED = true;
  bool message_is_ok = true;
  IPC_BEGIN_MESSAGE_MAP_EX(WebRTCAudioDeviceTest, message, message_is_ok)
    IPC_MESSAGE_HANDLER(ViewHostMsg_GetHardwareSampleRate,
                        OnGetHardwareSampleRate)
    IPC_MESSAGE_HANDLER(ViewHostMsg_GetHardwareInputSampleRate,
                        OnGetHardwareInputSampleRate)
    IPC_MESSAGE_HANDLER(ViewHostMsg_GetHardwareInputChannelCount,
                        OnGetHardwareInputChannelCount)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP_EX()

  EXPECT_TRUE(message_is_ok);

  return true;
}
