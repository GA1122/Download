  KURL RegisterMockedUrl(const std::string& url_root,
                         const WTF::String& filename) {
    WebURLResponse response;
    response.SetMIMEType("text/html");
    KURL url = ToKURL(url_root + filename.Utf8().data());
    Platform::Current()->GetURLLoaderMockFactory()->RegisterURL(
        url, response, test::CoreTestDataPath(filename.Utf8().data()));
    return url;
  }
