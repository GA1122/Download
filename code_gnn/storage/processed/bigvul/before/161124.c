void MediaStreamManager::FinalizeRequestFailed(
    const std::string& label,
    DeviceRequest* request,
    MediaStreamRequestResult result) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  switch (request->request_type) {
    case MEDIA_GENERATE_STREAM: {
      DCHECK(request->generate_stream_cb);
      std::move(request->generate_stream_cb)
          .Run(result, std::string(), MediaStreamDevices(),
               MediaStreamDevices());
      break;
    }
    case MEDIA_OPEN_DEVICE_PEPPER_ONLY: {
      DCHECK(request->open_device_cb);
      std::move(request->open_device_cb)
          .Run(false  , std::string(), MediaStreamDevice());
      break;
    }
    case MEDIA_DEVICE_ACCESS: {
      DCHECK(request->media_access_request_cb);
      std::move(request->media_access_request_cb)
          .Run(MediaStreamDevices(), std::move(request->ui_proxy));
      break;
    }
    default:
      NOTREACHED();
      break;
  }

  DeleteRequest(label);
}
