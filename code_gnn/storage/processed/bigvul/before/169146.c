void RenderFrameHostImpl::CreateAudioInputStreamFactory(
    mojom::RendererAudioInputStreamFactoryRequest request) {
  BrowserMainLoop* browser_main_loop = BrowserMainLoop::GetInstance();
  DCHECK(browser_main_loop);
  audio_input_stream_factory_ =
      RenderFrameAudioInputStreamFactoryHandle::CreateFactory(
          base::BindRepeating(&AudioInputDelegateImpl::Create,
                              media::AudioManager::Get(),
                              AudioMirroringManager::GetInstance(),
                              browser_main_loop->user_input_monitor(),
                              GetProcess()->GetID(), GetRoutingID()),
          browser_main_loop->media_stream_manager(), GetProcess()->GetID(),
          GetRoutingID(), std::move(request));
}
