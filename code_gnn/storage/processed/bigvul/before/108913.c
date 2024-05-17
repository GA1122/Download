void RenderViewImpl::OnAsyncFileOpened(
    base::PlatformFileError error_code,
    IPC::PlatformFileForTransit file_for_transit,
    int message_id) {
  pepper_delegate_.OnAsyncFileOpened(
      error_code,
      IPC::PlatformFileForTransitToPlatformFile(file_for_transit),
      message_id);
}
