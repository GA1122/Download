bool RenderProcessHostImpl::StartWebRTCEventLog(
    const base::FilePath& file_path) {
  return webrtc_eventlog_host_.StartWebRTCEventLog(file_path);
}
