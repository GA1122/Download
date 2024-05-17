bool HostNPScriptObject::Connect(const NPVariant* args,
                                 uint32_t arg_count,
                                 NPVariant* result) {
  CHECK_EQ(base::PlatformThread::CurrentId(), np_thread_id_);

  LOG(INFO) << "Connecting...";

  if (arg_count != 2) {
    SetException("connect: bad number of arguments");
    return false;
  }

  if (state_ != kDisconnected) {
    SetException("connect: can be called only when disconnected");
    return false;
  }

  std::string uid = StringFromNPVariant(args[0]);
  if (uid.empty()) {
    SetException("connect: bad uid argument");
    return false;
  }

  std::string auth_service_with_token = StringFromNPVariant(args[1]);
  std::string auth_token;
  std::string auth_service;
  ParseAuthTokenWithService(auth_service_with_token, &auth_token,
                            &auth_service);
  if (auth_token.empty()) {
    SetException("connect: auth_service_with_token argument has empty token");
    return false;
  }

  ReadPolicyAndConnect(uid, auth_token, auth_service);

  return true;
}
