void RendererSchedulerImpl::OnAudioStateChanged() {
  bool is_audio_playing = false;
  for (WebViewSchedulerImpl* web_view_scheduler :
       main_thread_only().web_view_schedulers) {
    is_audio_playing = is_audio_playing || web_view_scheduler->IsPlayingAudio();
  }

  if (is_audio_playing == main_thread_only().is_audio_playing)
    return;

  main_thread_only().last_audio_state_change = helper_.NowTicks();
  main_thread_only().is_audio_playing = is_audio_playing;

  UpdatePolicy();
}
