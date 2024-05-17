MojoAudioOutputIPC::~MojoAudioOutputIPC() {
  DCHECK(!AuthorizationRequested() && !StreamCreationRequested())
      << "CloseStream must be called before destructing the AudioOutputIPC";
}
