media::AudioOutputController* AudioRendererHost::LookupControllerByIdForTesting(
    int stream_id) {
  AudioEntry* const entry = LookupById(stream_id);
  return entry ? entry->controller : NULL;
}
