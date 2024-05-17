std::string TestURLLoader::TestXRequestedWithHeader() {
  pp::URLRequestInfo request(instance_);
  request.SetURL("/echoheader?X-Requested-With");
  return LoadAndCompareBody(request, "PPAPITests/1.2.3");
}
