void HTMLMediaElement::CancelledRemotePlaybackRequest() {
  if (RemotePlaybackClient())
    RemotePlaybackClient()->PromptCancelled();
}
