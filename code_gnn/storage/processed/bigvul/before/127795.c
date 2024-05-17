  bool SimulateEnumerateResult(
      int request_id,
      const std::vector<ppapi::DeviceRefData>& devices) {
    std::map<int, EnumerateDevicesCallback>::iterator iter =
        callbacks_.find(request_id);
    if (iter == callbacks_.end())
      return false;

    iter->second.Run(request_id, devices);
    return true;
  }
