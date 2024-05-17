  std::string MediaStreamManager::MakeMediaAccessRequest(
      int render_process_id,
      int render_frame_id,
//     int requester_id,
      int page_request_id,
      const StreamControls& controls,
      const url::Origin& security_origin,
      MediaAccessRequestCallback callback) {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);
  
    DeviceRequest* request = new DeviceRequest(
      render_process_id, render_frame_id, page_request_id,
//       render_process_id, render_frame_id, requester_id, page_request_id,
        false  , MEDIA_DEVICE_ACCESS, controls,
        MediaDeviceSaltAndOrigin{std::string()  ,
                                 std::string()  ,
                                security_origin});
 
   const std::string& label = AddRequest(request);
 
   request->media_access_request_cb = std::move(callback);
   base::PostTaskWithTraits(FROM_HERE, {BrowserThread::IO},
                            base::BindOnce(&MediaStreamManager::SetUpRequest,
                                           base::Unretained(this), label));
   return label;
 }