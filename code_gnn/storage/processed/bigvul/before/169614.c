int32_t TestURLLoader::PrepareFileForPost(
      const pp::FileRef& file_ref,
      const std::string& data,
      std::string* message) {
  TestCompletionCallback callback(instance_->pp_instance(), callback_type());
  pp::FileIO file_io(instance_);
  callback.WaitForResult(file_io.Open(file_ref,
                                      PP_FILEOPENFLAG_CREATE |
                                      PP_FILEOPENFLAG_TRUNCATE |
                                      PP_FILEOPENFLAG_WRITE,
                                      callback.GetCallback()));
  if (callback.failed()) {
    message->assign(callback.errors());
    return callback.result();
  }
  if (callback.result() != PP_OK) {
    message->assign("FileIO::Open failed.");
    return callback.result();
  }

  int32_t rv = WriteEntireBuffer(instance_->pp_instance(), &file_io, 0, data,
                                 callback_type());
  if (rv != PP_OK) {
    message->assign("FileIO::Write failed.");
    return rv;
  }

  return rv;
}
