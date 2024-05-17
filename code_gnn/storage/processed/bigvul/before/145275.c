void DisplaySourceCustomBindings::StartSession(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(1, args.Length());
  CHECK(args[0]->IsObject());

  v8::Isolate* isolate = context()->isolate();
  v8::Local<v8::Object> start_info = args[0].As<v8::Object>();

  v8::Local<v8::Value> sink_id_val =
      GetChildValue(start_info, "sinkId", isolate);
  CHECK(sink_id_val->IsInt32());
  const int sink_id = sink_id_val->ToInt32(isolate)->Value();
  if (GetDisplaySession(sink_id)) {
    isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(
        isolate, kSessionAlreadyStarted)));
    return;
  }

  v8::Local<v8::Value> video_stream_val =
      GetChildValue(start_info, "videoTrack", isolate);
  v8::Local<v8::Value> audio_stream_val =
      GetChildValue(start_info, "audioTrack", isolate);

  if ((video_stream_val->IsNull() || video_stream_val->IsUndefined()) &&
      (audio_stream_val->IsNull() || audio_stream_val->IsUndefined())) {
    isolate->ThrowException(v8::Exception::Error(
        v8::String::NewFromUtf8(isolate, kInvalidStreamArgs)));
    return;
  }

  blink::WebMediaStreamTrack audio_track, video_track;

  if (!video_stream_val->IsNull() && !video_stream_val->IsUndefined()) {
    CHECK(video_stream_val->IsObject());
    video_track =
        blink::WebDOMMediaStreamTrack::fromV8Value(
            video_stream_val).component();
    if (video_track.isNull()) {
      isolate->ThrowException(v8::Exception::Error(
          v8::String::NewFromUtf8(isolate, kInvalidStreamArgs)));
      return;
    }
  }
  if (!audio_stream_val->IsNull() && !audio_stream_val->IsUndefined()) {
    CHECK(audio_stream_val->IsObject());
    audio_track =
        blink::WebDOMMediaStreamTrack::fromV8Value(
            audio_stream_val).component();
    if (audio_track.isNull()) {
      isolate->ThrowException(v8::Exception::Error(
          v8::String::NewFromUtf8(isolate, kInvalidStreamArgs)));
      return;
    }
  }

  scoped_ptr<DisplaySourceAuthInfo> auth_info;
  v8::Local<v8::Value> auth_info_v8_val =
      GetChildValue(start_info, "authenticationInfo", isolate);
  if (!auth_info_v8_val->IsNull()) {
    CHECK(auth_info_v8_val->IsObject());
    scoped_ptr<V8ValueConverter> converter(V8ValueConverter::create());
    scoped_ptr<base::Value> auth_info_val(
        converter->FromV8Value(auth_info_v8_val, context()->v8_context()));
    CHECK(auth_info_val);
    auth_info = DisplaySourceAuthInfo::FromValue(*auth_info_val);
  }

  DisplaySourceSessionParams session_params;
  session_params.sink_id = sink_id;
  session_params.video_track = video_track;
  session_params.audio_track = audio_track;
  session_params.render_frame = context()->GetRenderFrame();
  if (auth_info) {
    session_params.auth_method = auth_info->method;
    session_params.auth_data = auth_info->data ? *auth_info->data : "";
  }
  scoped_ptr<DisplaySourceSession> session =
      DisplaySourceSessionFactory::CreateSession(session_params);
  if (!session) {
    isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(
        isolate, kErrorNotSupported)));
    return;
  }

  auto on_terminated_callback =
      base::Bind(&DisplaySourceCustomBindings::OnSessionTerminated,
                 weak_factory_.GetWeakPtr(), sink_id);
  auto on_error_callback =
      base::Bind(&DisplaySourceCustomBindings::OnSessionError,
                 weak_factory_.GetWeakPtr(), sink_id);
  session->SetNotificationCallbacks(on_terminated_callback, on_error_callback);

  int32_t call_id = GetCallbackId();
  args.GetReturnValue().Set(call_id);

  auto on_call_completed =
      base::Bind(&DisplaySourceCustomBindings::OnSessionStarted,
                 weak_factory_.GetWeakPtr(), sink_id, call_id);
  session->Start(on_call_completed);
  session_map_.insert(std::make_pair(sink_id, std::move(session)));
}
