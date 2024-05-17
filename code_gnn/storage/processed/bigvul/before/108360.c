void AudioRendererHost::DoCompleteCreation(
    media::AudioOutputController* controller) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  AudioEntry* entry = LookupByController(controller);
  if (!entry)
    return;

  if (!peer_handle()) {
    NOTREACHED() << "Renderer process handle is invalid.";
    DeleteEntryOnError(entry);
    return;
  }

  base::SharedMemoryHandle foreign_memory_handle;
  if (!entry->shared_memory.ShareToProcess(peer_handle(),
                                           &foreign_memory_handle)) {
    DeleteEntryOnError(entry);
    return;
  }

  if (entry->controller->LowLatencyMode()) {
    AudioSyncReader* reader =
        static_cast<AudioSyncReader*>(entry->reader.get());

#if defined(OS_WIN)
    base::SyncSocket::Handle foreign_socket_handle;
#else
    base::FileDescriptor foreign_socket_handle;
#endif

    if (!reader->PrepareForeignSocketHandle(peer_handle(),
                                            &foreign_socket_handle)) {
      DeleteEntryOnError(entry);
      return;
    }

    Send(new ViewMsg_NotifyLowLatencyAudioStreamCreated(
        entry->render_view_id, entry->stream_id, foreign_memory_handle,
        foreign_socket_handle, entry->shared_memory.created_size()));
    return;
  }

  Send(new ViewMsg_NotifyAudioStreamCreated(
      entry->render_view_id, entry->stream_id, foreign_memory_handle,
      entry->shared_memory.created_size()));
}
