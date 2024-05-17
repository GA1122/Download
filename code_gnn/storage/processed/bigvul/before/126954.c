void AudioInputRendererHost::OnChannelClosing() {
  BrowserMessageFilter::OnChannelClosing();

  DeleteEntries();
}
