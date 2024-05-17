int32_t TestURLLoader::Open(const pp::URLRequestInfo& request,
                            bool trusted,
                            std::string* response_body) {
  pp::URLLoader loader(instance_);
  if (trusted)
    url_loader_trusted_interface_->GrantUniversalAccess(loader.pp_resource());

  {
    TestCompletionCallback open_callback(instance_->pp_instance(),
                                         callback_type());
    open_callback.WaitForResult(
        loader.Open(request, open_callback.GetCallback()));
    if (open_callback.result() != PP_OK)
      return open_callback.result();
  }

  int32_t bytes_read = 0;
  do {
    char buffer[1024];
    TestCompletionCallback read_callback(instance_->pp_instance(),
                                         callback_type());
    read_callback.WaitForResult(loader.ReadResponseBody(
        &buffer, sizeof(buffer), read_callback.GetCallback()));
    bytes_read = read_callback.result();
    if (bytes_read < 0)
      return bytes_read;
    if (response_body)
      response_body->append(std::string(buffer, bytes_read));
  } while (bytes_read > 0);

  return PP_OK;
}
