void RenderFrameHostImpl::CreateAudioInputStreamFactory(
    mojom::RendererAudioInputStreamFactoryRequest request) {
  BrowserMainLoop* browser_main_loop = BrowserMainLoop::GetInstance();
  DCHECK(browser_main_loop);
  if (base::FeatureList::IsEnabled(features::kAudioServiceAudioStreams)) {
    MediaStreamManager* msm = browser_main_loop->media_stream_manager();
    audio_service_audio_input_stream_factory_.emplace(std::move(request), msm,
                                                      this);
  } else {
    in_content_audio_input_stream_factory_ =
        RenderFrameAudioInputStreamFactoryHandle::CreateFactory(
            base::BindRepeating(&AudioInputDelegateImpl::Create,
                                browser_main_loop->audio_manager(),
                                AudioMirroringManager::GetInstance(),
                                browser_main_loop->user_input_monitor(),
                                GetProcess()->GetID(), GetRoutingID()),
            browser_main_loop->media_stream_manager(), GetProcess()->GetID(),
            GetRoutingID(), std::move(request));
  }
}
