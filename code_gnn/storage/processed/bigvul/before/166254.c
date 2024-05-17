void VideoCaptureManager::DestroyControllerIfNoClients(
    VideoCaptureController* controller) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (!controller->HasActiveClient() && !controller->HasPausedClient()) {
    std::ostringstream string_stream;
    string_stream << "VideoCaptureManager stopping device (stream_type = "
                  << controller->stream_type()
                  << ", device_id = " << controller->device_id() << ")";
    EmitLogMessage(string_stream.str(), 1);

    DoStopDevice(controller);
    auto controller_iter = std::find_if(
        controllers_.begin(), controllers_.end(),
        [controller](
            const scoped_refptr<VideoCaptureController>& device_entry) {
          return device_entry.get() == controller;
        });
    controllers_.erase(controller_iter);
  }
}
