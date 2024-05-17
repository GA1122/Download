  void SimulateMonitorDeviceChangeReceived(uint32_t callback_id) {
    PpapiHostMsg_DeviceEnumeration_MonitorDeviceChange msg(callback_id);
    ppapi::proxy::ResourceMessageCallParams call_params(
        resource_host_.pp_resource(), 123);
    ppapi::host::HostMessageContext context(call_params);
    int32_t result = PP_ERROR_FAILED;
    ASSERT_TRUE(
        device_enumeration_.HandleResourceMessage(msg, &context, &result));
    EXPECT_EQ(PP_OK, result);
  }
