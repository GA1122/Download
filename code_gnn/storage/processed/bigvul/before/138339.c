void MediaInterfaceProxy::CreateAudioDecoder(
    media::mojom::AudioDecoderRequest request) {
  DCHECK(thread_checker_.CalledOnValidThread());
  GetMediaInterfaceFactory()->CreateAudioDecoder(std::move(request));
}
