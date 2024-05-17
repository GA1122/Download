void ClassicPendingScript::DataReceived(Resource* resource,
                                        const char*,
                                        size_t) {
  if (streamer_)
    streamer_->NotifyAppendData(ToScriptResource(resource));
}
