std::string TestURLLoader::TestStreamToFile() {
  pp::URLRequestInfo request(instance_);
  request.SetURL("test_url_loader_data/hello.txt");
  ASSERT_FALSE(request.SetStreamToFile(true));

  TestCompletionCallback callback(instance_->pp_instance(), callback_type());

  pp::URLLoader loader(instance_);
  callback.WaitForResult(loader.Open(request, callback.GetCallback()));
  CHECK_CALLBACK_BEHAVIOR(callback);
  ASSERT_EQ(PP_OK, callback.result());

  pp::URLResponseInfo response_info(loader.GetResponseInfo());
  if (response_info.is_null())
    return "URLLoader::GetResponseInfo returned null";
  int32_t status_code = response_info.GetStatusCode();
  if (status_code != 200)
    return "Unexpected HTTP status code";

  pp::FileRef body(response_info.GetBodyAsFileRef());
  ASSERT_TRUE(body.is_null());

  callback.WaitForResult(loader.FinishStreamingToFile(callback.GetCallback()));
  CHECK_CALLBACK_BEHAVIOR(callback);
  ASSERT_EQ(PP_ERROR_NOTSUPPORTED, callback.result());

  PASS();
}
