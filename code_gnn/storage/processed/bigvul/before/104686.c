  static v8::Handle<v8::Value> GetLocalFileSystem(
      const v8::Arguments& args) {
    DCHECK(args.Length() == 2);
    DCHECK(args[0]->IsString());
    DCHECK(args[1]->IsString());
    std::string name(*v8::String::Utf8Value(args[0]));
    std::string path(*v8::String::Utf8Value(args[1]));

    WebFrame* webframe = WebFrame::frameForCurrentContext();
#ifdef WEB_FILE_SYSTEM_TYPE_EXTERNAL
    return webframe->createFileSystem(WebKit::WebFileSystem::TypeExternal,
            WebKit::WebString::fromUTF8(name.c_str()),
            WebKit::WebString::fromUTF8(path.c_str()));
#else
    return webframe->createFileSystem(fileapi::kFileSystemTypeExternal,
            WebKit::WebString::fromUTF8(name.c_str()),
            WebKit::WebString::fromUTF8(path.c_str()));
#endif
  }
