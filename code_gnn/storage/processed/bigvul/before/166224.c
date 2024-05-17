DesktopMediaID MediaStreamManager::ResolveTabCaptureDeviceIdOnUIThread(
    const std::string& capture_device_id,
    int requesting_process_id,
    int requesting_frame_id,
    const GURL& origin) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  return DesktopStreamsRegistry::GetInstance()->RequestMediaForStreamId(
      capture_device_id, requesting_process_id, requesting_frame_id, origin,
      nullptr, kRegistryStreamTypeTab);
}
