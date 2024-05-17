RenderThread::RenderThread(const std::string& channel_name)
    : ChildThread(channel_name) {
  Init();
}
