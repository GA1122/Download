int32_t TestURLLoader::OpenFileSystem(pp::FileSystem* file_system,
                                      std::string* message) {
  TestCompletionCallback callback(instance_->pp_instance(), callback_type());
  callback.WaitForResult(file_system->Open(1024, callback.GetCallback()));
  if (callback.failed()) {
    message->assign(callback.errors());
    return callback.result();
  }
  if (callback.result() != PP_OK) {
    message->assign("FileSystem::Open");
    return callback.result();
  }
  return callback.result();
}
