void DevToolsUIBindings::SendMessageAck(int request_id,
                                        const base::Value* arg) {
  base::Value id_value(request_id);
  CallClientFunction("DevToolsAPI.embedderMessageAck",
                     &id_value, arg, nullptr);
}
