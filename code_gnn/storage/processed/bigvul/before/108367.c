void AudioRendererHost::OnChannelClosing() {
  BrowserMessageFilter::OnChannelClosing();

  DeleteEntries();
}
