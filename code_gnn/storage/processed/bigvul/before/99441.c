void RemoveAndCloseChannelSocket(const std::string& name) {
  Singleton<PipeMap>()->RemoveAndClose(name);
}
