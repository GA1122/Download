std::string TestURLLoader::ReadEntireFile(pp::FileIO* file_io,
                                          std::string* data) {
  TestCompletionCallback callback(instance_->pp_instance(), callback_type());
  char buf[256];
  int64_t offset = 0;

  for (;;) {
    callback.WaitForResult(file_io->Read(offset, buf, sizeof(buf),
                           callback.GetCallback()));
    if (callback.result() < 0)
      return ReportError("FileIO::Read", callback.result());
    if (callback.result() == 0)
      break;
    offset += callback.result();
    data->append(buf, callback.result());
  }

  PASS();
}
