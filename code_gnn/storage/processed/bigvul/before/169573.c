void CastStreamingNativeHandler::CreateCastSession(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(3, args.Length());
  CHECK(args[2]->IsFunction());

  v8::Isolate* isolate = context()->v8_context()->GetIsolate();
  if ((args[0]->IsNull() || args[0]->IsUndefined()) &&
      (args[1]->IsNull() || args[1]->IsUndefined())) {
    isolate->ThrowException(v8::Exception::Error(
        v8::String::NewFromUtf8(isolate, kInvalidStreamArgs)));
    return;
  }

  scoped_refptr<CastSession> session(new CastSession());
  scoped_ptr<CastRtpStream> stream1, stream2;
  if (!args[0]->IsNull() && !args[0]->IsUndefined()) {
    CHECK(args[0]->IsObject());
    blink::WebDOMMediaStreamTrack track =
        blink::WebDOMMediaStreamTrack::fromV8Value(args[0]);
    if (track.isNull()) {
      isolate->ThrowException(v8::Exception::Error(
          v8::String::NewFromUtf8(isolate, kInvalidStreamArgs)));
      return;
    }
    stream1.reset(new CastRtpStream(track.component(), session));
  }
  if (!args[1]->IsNull() && !args[1]->IsUndefined()) {
    CHECK(args[1]->IsObject());
    blink::WebDOMMediaStreamTrack track =
        blink::WebDOMMediaStreamTrack::fromV8Value(args[1]);
    if (track.isNull()) {
      isolate->ThrowException(v8::Exception::Error(
          v8::String::NewFromUtf8(isolate, kInvalidStreamArgs)));
      return;
    }
    stream2.reset(new CastRtpStream(track.component(), session));
  }
  scoped_ptr<CastUdpTransport> udp_transport(
      new CastUdpTransport(session));

  create_callback_.Reset(isolate, args[2].As<v8::Function>());

  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE,
      base::Bind(&CastStreamingNativeHandler::CallCreateCallback,
                 weak_factory_.GetWeakPtr(), base::Passed(&stream1),
                 base::Passed(&stream2), base::Passed(&udp_transport)));
}
