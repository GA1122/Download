void AudioRendererHost::OnCreateStream(
    const IPC::Message& msg, int stream_id,
    const ViewHostMsg_Audio_CreateStream_Params& params, bool low_latency) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  DCHECK(LookupById(msg.routing_id(), stream_id) == NULL);

  AudioParameters audio_params(params.params);

  if (!audio_params.samples_per_packet) {
    audio_params.samples_per_packet = SelectSamplesPerPacket(audio_params);
  }
  uint32 packet_size = audio_params.GetPacketSize();

  scoped_ptr<AudioEntry> entry(new AudioEntry());
  if (!entry->shared_memory.CreateAndMapAnonymous(packet_size)) {
    SendErrorMessage(msg.routing_id(), stream_id);
    return;
  }

  if (low_latency) {
    scoped_ptr<AudioSyncReader> reader(
        new AudioSyncReader(&entry->shared_memory));

    if (!reader->Init()) {
      SendErrorMessage(msg.routing_id(), stream_id);
      return;
    }

    entry->reader.reset(reader.release());
    entry->controller =
        media::AudioOutputController::CreateLowLatency(this, audio_params,
                                                       entry->reader.get());
  } else {
    entry->controller =
        media::AudioOutputController::Create(this, audio_params,
                                             3 * packet_size);
  }

  if (!entry->controller) {
    SendErrorMessage(msg.routing_id(), stream_id);
    return;
  }

  entry->render_view_id = msg.routing_id();
  entry->stream_id = stream_id;

  audio_entries_.insert(std::make_pair(
      AudioEntryId(msg.routing_id(), stream_id),
      entry.release()));
}
