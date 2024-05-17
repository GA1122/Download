std::string TestURLLoader::TestBinaryDataPOST() {
  pp::URLRequestInfo request(instance_);
  request.SetURL("/echo");
  request.SetMethod("POST");
  const char postdata_chars[] =
      "\x00\x01\x02\x03\x04\x05postdata\xfa\xfb\xfc\xfd\xfe\xff";
  std::string postdata(postdata_chars,
                       sizeof(postdata_chars) / sizeof(postdata_chars[0]));
  request.AppendDataToBody(postdata.data(),
                           static_cast<uint32_t>(postdata.length()));
  return LoadAndCompareBody(request, postdata);
}
