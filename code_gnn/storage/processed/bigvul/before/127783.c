  void EnumerateDevicesCallbackBody(
      int request_id,
      const std::vector<ppapi::DeviceRefData>& devices) {
    if (sync_call_) {
      base::MessageLoop::current()->PostTask(
          FROM_HERE,
          base::Bind(&ScopedRequest::EnumerateDevicesCallbackBody,
                     AsWeakPtr(),
                     request_id,
                     devices));
    } else {
      DCHECK_EQ(request_id_, request_id);
      callback_.Run(request_id, devices);
    }
  }
