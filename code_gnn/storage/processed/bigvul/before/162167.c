MojoAudioInputIPC::MojoAudioInputIPC(StreamCreatorCB stream_creator)
    : stream_creator_(std::move(stream_creator)),
      stream_client_binding_(this),
      factory_client_binding_(this),
      weak_factory_(this) {
  DETACH_FROM_SEQUENCE(sequence_checker_);
  DCHECK(stream_creator_);
}
