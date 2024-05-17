void MediaInterfaceProxy::CreateVideoDecoder(
    media::mojom::VideoDecoderRequest request) {
  DCHECK(thread_checker_.CalledOnValidThread());
  GetMediaInterfaceFactory()->CreateVideoDecoder(std::move(request));
}
