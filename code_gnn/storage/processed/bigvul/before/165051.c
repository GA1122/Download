void HTMLCanvasElement::toBlob(V8BlobCallback* callback,
                               const String& mime_type,
                               const ScriptValue& quality_argument,
                               ExceptionState& exception_state) {
  if (!OriginClean()) {
    exception_state.ThrowSecurityError("Tainted canvases may not be exported.");
    return;
  }

  if (!IsPaintable()) {
    GetDocument()
        .GetTaskRunner(TaskType::kCanvasBlobSerialization)
        ->PostTask(
            FROM_HERE,
            WTF::Bind(&V8PersistentCallbackFunction<
                          V8BlobCallback>::InvokeAndReportException,
                      WrapPersistent(ToV8PersistentCallbackFunction(callback)),
                      nullptr, nullptr));
    return;
  }

  TimeTicks start_time = WTF::CurrentTimeTicks();
  double quality = kUndefinedQualityValue;
  if (!quality_argument.IsEmpty()) {
    v8::Local<v8::Value> v8_value = quality_argument.V8Value();
    if (v8_value->IsNumber())
      quality = v8_value.As<v8::Number>()->Value();
  }

  ImageEncodingMimeType encoding_mime_type =
      ImageEncoderUtils::ToEncodingMimeType(
          mime_type, ImageEncoderUtils::kEncodeReasonToBlobCallback);

  CanvasAsyncBlobCreator* async_creator = nullptr;
  scoped_refptr<StaticBitmapImage> image_bitmap =
      Snapshot(kBackBuffer, kPreferNoAcceleration);
  if (image_bitmap) {
    async_creator = CanvasAsyncBlobCreator::Create(
        image_bitmap, encoding_mime_type, callback,
        CanvasAsyncBlobCreator::kHTMLCanvasToBlobCallback, start_time,
        &GetDocument());
  }

  if (async_creator) {
    async_creator->ScheduleAsyncBlobCreation(quality);
  } else {
    GetDocument()
        .GetTaskRunner(TaskType::kCanvasBlobSerialization)
        ->PostTask(
            FROM_HERE,
            WTF::Bind(&V8PersistentCallbackFunction<
                          V8BlobCallback>::InvokeAndReportException,
                      WrapPersistent(ToV8PersistentCallbackFunction(callback)),
                      nullptr, nullptr));
  }
}
