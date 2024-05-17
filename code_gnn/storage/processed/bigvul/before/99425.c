bool ChannelSocketExists(const std::string& name) {
  return Singleton<PipeMap>()->Lookup(name) != -1;
}
