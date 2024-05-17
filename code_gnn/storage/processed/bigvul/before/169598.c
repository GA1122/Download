void MediaGalleriesCustomBindings::GetMediaFileSystemObject(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(1, args.Length());
  CHECK(args[0]->IsString());

  std::string fs_mount(*v8::String::Utf8Value(args[0]));
  CHECK(!fs_mount.empty());

  blink::WebLocalFrame* webframe =
      blink::WebLocalFrame::frameForCurrentContext();
  const GURL origin = blink::WebStringToGURL(
      webframe->document().getSecurityOrigin().toString());
  std::string fs_name =
      storage::GetFileSystemName(origin, storage::kFileSystemTypeExternal);
  fs_name.append("_");
  fs_name.append(fs_mount);
  const GURL root_url(
      storage::GetExternalFileSystemRootURIString(origin, fs_mount));
  args.GetReturnValue().Set(
      blink::WebDOMFileSystem::create(webframe,
                                      blink::WebFileSystemTypeExternal,
                                      blink::WebString::fromUTF8(fs_name),
                                      root_url)
          .toV8Value(context()->v8_context()->Global(), args.GetIsolate()));
}
