std::string TestURLLoader::TestAbortCalls() {
  pp::URLRequestInfo request(instance_);
  request.SetURL("test_url_loader_data/hello.txt");

  TestCompletionCallback callback(instance_->pp_instance(), callback_type());
  int32_t rv;

  {
    rv = pp::URLLoader(instance_).Open(request, callback.GetCallback());
  }
  callback.WaitForAbortResult(rv);
  CHECK_CALLBACK_BEHAVIOR(callback);

  {
    char buf[2] = { 0 };
    {
      pp::URLLoader loader(instance_);
      callback.WaitForResult(loader.Open(request, callback.GetCallback()));
      CHECK_CALLBACK_BEHAVIOR(callback);
      ASSERT_EQ(PP_OK, callback.result());

      rv = loader.ReadResponseBody(buf, sizeof(buf), callback.GetCallback());
    }   
    callback.WaitForAbortResult(rv);
    CHECK_CALLBACK_BEHAVIOR(callback);
    if (rv == PP_OK_COMPLETIONPENDING) {
      if (buf[0] || buf[1]) {
        return "URLLoader::ReadResponseBody wrote data after resource "
               "destruction.";
      }
    }
  }


  PASS();
}
