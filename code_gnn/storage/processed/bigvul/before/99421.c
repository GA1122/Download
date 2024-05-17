void AddChannelSocket(const std::string& name, int socket) {
  Singleton<PipeMap>()->Insert(name, socket);
}
