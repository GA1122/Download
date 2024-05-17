bool AudioRendererHost::OnMessageReceived(const IPC::Message& message,
                                          bool* message_was_ok) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP_EX(AudioRendererHost, message, *message_was_ok)
    IPC_MESSAGE_HANDLER(ViewHostMsg_CreateAudioStream, OnCreateStream)
    IPC_MESSAGE_HANDLER(ViewHostMsg_PlayAudioStream, OnPlayStream)
    IPC_MESSAGE_HANDLER(ViewHostMsg_PauseAudioStream, OnPauseStream)
    IPC_MESSAGE_HANDLER(ViewHostMsg_FlushAudioStream, OnFlushStream)
    IPC_MESSAGE_HANDLER(ViewHostMsg_CloseAudioStream, OnCloseStream)
    IPC_MESSAGE_HANDLER(ViewHostMsg_NotifyAudioPacketReady, OnNotifyPacketReady)
    IPC_MESSAGE_HANDLER(ViewHostMsg_GetAudioVolume, OnGetVolume)
    IPC_MESSAGE_HANDLER(ViewHostMsg_SetAudioVolume, OnSetVolume)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP_EX()

  return handled;
}
