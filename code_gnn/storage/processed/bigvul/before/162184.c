bool MojoAudioOutputIPC::StreamCreationRequested() {
  return stream_.is_bound();
}
