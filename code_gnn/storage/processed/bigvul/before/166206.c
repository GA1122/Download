void MediaStreamManager::FinishTabCaptureRequestSetupWithDeviceId(
    const std::string& label,
    const DesktopMediaID& device_id) {
  DCHECK_CURRENTLY_ON(content::BrowserThread::IO);

  DeviceRequest* request = FindRequest(label);
  if (!request) {
    DVLOG(1) << "SetUpRequest label " << label << " doesn't exist!!";
    return;   
  }

  if (device_id.type != content::DesktopMediaID::TYPE_WEB_CONTENTS) {
    FinalizeRequestFailed(label, request, MEDIA_DEVICE_TAB_CAPTURE_FAILURE);
    return;
  }

  content::WebContentsMediaCaptureId web_id = device_id.web_contents_id;
  web_id.disable_local_echo = request->controls.disable_local_echo;

  request->tab_capture_device_id = web_id.ToString();

  request->CreateTabCaptureUIRequest(web_id.render_process_id,
                                     web_id.main_render_frame_id);

  DVLOG(3) << "SetUpTabCaptureRequest "
           << ", {capture_device_id = " << web_id.ToString() << "}"
           << ", {target_render_process_id = " << web_id.render_process_id
           << "}"
           << ", {target_render_frame_id = " << web_id.main_render_frame_id
           << "}";

  ReadOutputParamsAndPostRequestToUI(label, request, MediaDeviceEnumeration());
}
