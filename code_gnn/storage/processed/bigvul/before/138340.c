void MediaInterfaceProxy::CreateRenderer(
    const std::string& audio_device_id,
    media::mojom::RendererRequest request) {
  DCHECK(thread_checker_.CalledOnValidThread());
  GetMediaInterfaceFactory()->CreateRenderer(audio_device_id,
                                             std::move(request));
}
