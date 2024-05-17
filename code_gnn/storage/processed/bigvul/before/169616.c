std::string TestURLLoader::ReadEntireResponseBody(pp::URLLoader* loader,
                                                  std::string* body) {
  TestCompletionCallback callback(instance_->pp_instance(), callback_type());
  char buf[2];   

  for (;;) {
    callback.WaitForResult(
        loader->ReadResponseBody(buf, sizeof(buf), callback.GetCallback()));
    if (callback.result() < 0)
      return ReportError("URLLoader::ReadResponseBody", callback.result());
    if (callback.result() == 0)
      break;
    body->append(buf, callback.result());
  }

  PASS();
}
