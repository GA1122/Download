bool MojoAudioOutputIPC::AuthorizationRequested() {
  return stream_provider_.is_bound();
}
