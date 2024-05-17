void BlinkTestRunner::LoadURLForFrame(const WebURL& url,
                                      const std::string& frame_name) {
  Send(new ShellViewHostMsg_LoadURLForFrame(
      routing_id(), url, frame_name));
}
