void MojoAudioOutputIPC::CreateStream(media::AudioOutputIPCDelegate* delegate,
                                      const media::AudioParameters& params) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(delegate);
  DCHECK(!StreamCreationRequested());
  if (!AuthorizationRequested()) {
    DCHECK(!delegate_);
    delegate_ = delegate;
    if (!DoRequestDeviceAuthorization(
            0, media::AudioDeviceDescription::kDefaultDeviceId,
            base::BindOnce(&TrivialAuthorizedCallback))) {
      return;
    }
  }

  DCHECK_EQ(delegate_, delegate);
  media::mojom::AudioOutputStreamClientPtr client_ptr;
  binding_.Bind(mojo::MakeRequest(&client_ptr));
  stream_provider_->Acquire(mojo::MakeRequest(&stream_), std::move(client_ptr),
                            params,
                            base::BindOnce(&MojoAudioOutputIPC::StreamCreated,
                                           base::Unretained(this)));

}
