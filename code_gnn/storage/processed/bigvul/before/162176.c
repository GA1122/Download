MojoAudioOutputIPC::MakeProviderRequest() {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(!AuthorizationRequested());
  media::mojom::AudioOutputStreamProviderRequest request =
      mojo::MakeRequest(&stream_provider_);

  return request;
}
