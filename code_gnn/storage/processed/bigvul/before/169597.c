 void FileManagerPrivateCustomBindings::GetFileSystem(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  DCHECK(args.Length() == 2);
  DCHECK(args[0]->IsString());
  DCHECK(args[1]->IsString());
  std::string name(*v8::String::Utf8Value(args[0]));
  std::string root_url(*v8::String::Utf8Value(args[1]));

  blink::WebLocalFrame* webframe =
      blink::WebLocalFrame::frameForContext(context()->v8_context());
  DCHECK(webframe);
  args.GetReturnValue().Set(
      blink::WebDOMFileSystem::create(webframe,
                                      blink::WebFileSystemTypeExternal,
                                      blink::WebString::fromUTF8(name),
                                      GURL(root_url))
           .toV8Value(context()->v8_context()->Global(), args.GetIsolate()));
 }
