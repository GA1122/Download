std::string TestURLLoader::TestCustomRequestHeader() {
  pp::URLRequestInfo request(instance_);
  request.SetURL("/echoheader?Foo");
  request.SetHeaders("Foo: 1");
  return LoadAndCompareBody(request, "1");
}
