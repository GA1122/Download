  void CheckNotifyDeviceChangeMessage(
      uint32_t callback_id,
      const std::vector<ppapi::DeviceRefData>& expected) {
    ppapi::proxy::ResourceMessageReplyParams reply_params;
    IPC::Message reply_msg;
    ASSERT_TRUE(sink_.GetFirstResourceReplyMatching(
        PpapiPluginMsg_DeviceEnumeration_NotifyDeviceChange::ID,
        &reply_params,
        &reply_msg));
    sink_.ClearMessages();

    EXPECT_EQ(PP_OK, reply_params.result());

    uint32_t reply_callback_id = 0;
    std::vector<ppapi::DeviceRefData> reply_data;
    ASSERT_TRUE(ppapi::UnpackMessage<
        PpapiPluginMsg_DeviceEnumeration_NotifyDeviceChange>(
        reply_msg, &reply_callback_id, &reply_data));
    EXPECT_EQ(callback_id, reply_callback_id);
    EXPECT_EQ(expected, reply_data);
  }
