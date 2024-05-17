void CastStreamingNativeHandler::StartCastRtpReceiver(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() < 8 || args.Length() > 9 ||
      !args[0]->IsObject() ||
      !args[1]->IsObject() ||
      !args[2]->IsObject() ||
      !args[3]->IsInt32() ||
      !args[4]->IsInt32() ||
      !args[5]->IsNumber() ||
      !args[6]->IsString()) {
    args.GetIsolate()->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(args.GetIsolate(), kUnableToConvertArgs)));
    return;
  }

  v8::Isolate* isolate = context()->v8_context()->GetIsolate();

  scoped_refptr<CastReceiverSession> session(
      new CastReceiverSession());
  media::cast::FrameReceiverConfig audio_config;
  media::cast::FrameReceiverConfig video_config;
  net::IPEndPoint local_endpoint;
  net::IPEndPoint remote_endpoint;

  if (!FrameReceiverConfigFromArg(isolate, args[0], &audio_config) ||
      !FrameReceiverConfigFromArg(isolate, args[1], &video_config) ||
      !IPEndPointFromArg(isolate, args[2], &local_endpoint)) {
    return;
  }

  const int max_width = args[3]->ToInt32(args.GetIsolate())->Value();
  const int max_height = args[4]->ToInt32(args.GetIsolate())->Value();
  const double fps = args[5]->NumberValue();

  if (fps <= 1) {
    args.GetIsolate()->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(args.GetIsolate(), kInvalidFPS)));
    return;
  }

  const std::string url = *v8::String::Utf8Value(args[6]);
  blink::WebMediaStream stream =
      blink::WebMediaStreamRegistry::lookupMediaStreamDescriptor(GURL(url));

  if (stream.isNull()) {
    args.GetIsolate()->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(args.GetIsolate(), kInvalidMediaStreamURL)));
    return;
  }

  media::VideoCaptureFormat capture_format(gfx::Size(max_width, max_height),
                                           fps, media::PIXEL_FORMAT_I420);

  video_config.target_frame_rate = fps;
  audio_config.target_frame_rate = 100;

  media::AudioParameters params(
      media::AudioParameters::AUDIO_PCM_LINEAR,
      media::GuessChannelLayout(audio_config.channels),
      audio_config.rtp_timebase,   
      16, audio_config.rtp_timebase / audio_config.target_frame_rate);

  if (!params.IsValid()) {
    args.GetIsolate()->ThrowException(v8::Exception::TypeError(
        v8::String::NewFromUtf8(args.GetIsolate(), kInvalidAudioParams)));
    return;
  }

  scoped_ptr<base::DictionaryValue> options;
  if (args.Length() >= 9) {
    scoped_ptr<V8ValueConverter> converter(V8ValueConverter::create());
    scoped_ptr<base::Value> options_value(
        converter->FromV8Value(args[8], context()->v8_context()));
    if (!options_value->IsType(base::Value::TYPE_NULL)) {
      options = base::DictionaryValue::From(std::move(options_value));
      if (!options) {
        args.GetIsolate()->ThrowException(v8::Exception::TypeError(
            v8::String::NewFromUtf8(args.GetIsolate(), kUnableToConvertArgs)));
        return;
      }
    }
  }

  if (!options) {
    options.reset(new base::DictionaryValue());
  }

  v8::CopyablePersistentTraits<v8::Function>::CopyablePersistent error_callback;
  error_callback.Reset(args.GetIsolate(),
                       v8::Local<v8::Function>(args[7].As<v8::Function>()));

  session->Start(
      audio_config, video_config, local_endpoint, remote_endpoint,
      std::move(options), capture_format,
      base::Bind(&CastStreamingNativeHandler::AddTracksToMediaStream,
                 weak_factory_.GetWeakPtr(), url, params),
      base::Bind(&CastStreamingNativeHandler::CallReceiverErrorCallback,
                 weak_factory_.GetWeakPtr(), error_callback));
}
