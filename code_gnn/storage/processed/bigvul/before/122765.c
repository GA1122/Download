void BrowserPluginGuest::SendMessageToEmbedder(IPC::Message* msg) {
  embedder_web_contents_->Send(msg);
}
