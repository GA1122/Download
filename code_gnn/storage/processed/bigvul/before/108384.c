void AudioRendererHost::SendErrorMessage(int32 render_view_id,
                                         int32 stream_id) {
  ViewMsg_AudioStreamState_Params state;
  state.state = ViewMsg_AudioStreamState_Params::kError;
  Send(new ViewMsg_NotifyAudioStreamStateChanged(
      render_view_id, stream_id, state));
}
