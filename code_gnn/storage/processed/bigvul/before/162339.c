MojoAudioInputStream::MojoAudioInputStream(
    mojom::AudioInputStreamRequest request,
    mojom::AudioInputStreamClientPtr client,
    CreateDelegateCallback create_delegate_callback,
    StreamCreatedCallback stream_created_callback,
    base::OnceClosure deleter_callback)
    : stream_created_callback_(std::move(stream_created_callback)),
      deleter_callback_(std::move(deleter_callback)),
      binding_(this, std::move(request)),
      client_(std::move(client)),
      weak_factory_(this) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  DCHECK(stream_created_callback_);
  DCHECK(deleter_callback_);
  binding_.set_connection_error_handler(
      base::BindOnce(&MojoAudioInputStream::OnError, base::Unretained(this)));
  client_.set_connection_error_handler(
      base::BindOnce(&MojoAudioInputStream::OnError, base::Unretained(this)));
  delegate_ = std::move(create_delegate_callback).Run(this);
  if (!delegate_) {
    binding_.Close();
    base::ThreadTaskRunnerHandle::Get()->PostTask(
        FROM_HERE,
        base::BindOnce(&MojoAudioInputStream::OnStreamError,
                       weak_factory_.GetWeakPtr(),   0));
  }
}
