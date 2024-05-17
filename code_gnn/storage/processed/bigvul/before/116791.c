void WebRTCAudioDeviceTest::CreateChannel(const char* name) {
  DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::IO));
  audio_render_host_ = new AudioRendererHost(
      resource_context_.get(), audio_manager_.get());
  audio_render_host_->OnChannelConnected(base::GetCurrentProcId());

  audio_input_renderer_host_ = new AudioInputRendererHost(
      resource_context_.get(), audio_manager_.get());
  audio_input_renderer_host_->OnChannelConnected(base::GetCurrentProcId());

  channel_.reset(new IPC::Channel(name, IPC::Channel::MODE_SERVER, this));
  ASSERT_TRUE(channel_->Connect());

  audio_render_host_->OnFilterAdded(channel_.get());
  audio_input_renderer_host_->OnFilterAdded(channel_.get());
}
