std::string TestURLLoader::TestBasicFileRangePOST() {
  std::string message;

  pp::FileSystem file_system(instance_, PP_FILESYSTEMTYPE_LOCALTEMPORARY);
  int32_t rv = OpenFileSystem(&file_system, &message);
  if (rv != PP_OK)
    return ReportError(message.c_str(), rv);

  pp::FileRef file_ref(file_system, "/file_range_post_test");
  std::string postdata("postdatapostdata");
  rv = PrepareFileForPost(file_ref, postdata, &message);
  if (rv != PP_OK)
    return ReportError(message.c_str(), rv);

  pp::URLRequestInfo request(instance_);
  request.SetURL("/echo");
  request.SetMethod("POST");
  request.AppendFileRangeToBody(file_ref, 4, 12, 0);
  return LoadAndCompareBody(request, postdata.substr(4, 12));
}
