ImageData* BaseRenderingContext2D::getImageData(
    int sx,
    int sy,
    int sw,
    int sh,
    ExceptionState& exception_state) {
  if (!WTF::CheckMul(sw, sh).IsValid<int>()) {
    exception_state.ThrowRangeError("Out of memory at ImageData creation");
    return nullptr;
  }

  usage_counters_.num_get_image_data_calls++;
  usage_counters_.area_get_image_data_calls += sw * sh;
  if (!OriginClean()) {
    exception_state.ThrowSecurityError(
        "The canvas has been tainted by cross-origin data.");
  } else if (!sw || !sh) {
    exception_state.ThrowDOMException(
        kIndexSizeError,
        String::Format("The source %s is 0.", sw ? "height" : "width"));
  }

  if (exception_state.HadException())
    return nullptr;

  if (sw < 0) {
    sx += sw;
    sw = -sw;
  }
  if (sh < 0) {
    sy += sh;
    sh = -sh;
  }

  if (!WTF::CheckAdd(sx, sw).IsValid<int>() ||
      !WTF::CheckAdd(sy, sh).IsValid<int>()) {
    exception_state.ThrowRangeError("Out of memory at ImageData creation");
    return nullptr;
  }

  Optional<ScopedUsHistogramTimer> timer;
  if (!IsPaint2D()) {
    if (GetImageBuffer() && GetImageBuffer()->IsAccelerated()) {
      DEFINE_THREAD_SAFE_STATIC_LOCAL(
          CustomCountHistogram, scoped_us_counter_gpu,
          ("Blink.Canvas.GetImageData.GPU", 0, 10000000, 50));
      timer.emplace(scoped_us_counter_gpu);
    } else {
      DEFINE_THREAD_SAFE_STATIC_LOCAL(
          CustomCountHistogram, scoped_us_counter_cpu,
          ("Blink.Canvas.GetImageData.CPU", 0, 10000000, 50));
      timer.emplace(scoped_us_counter_cpu);
    }
  }

  IntRect image_data_rect(sx, sy, sw, sh);
  ImageBuffer* buffer = GetImageBuffer();
  ImageDataColorSettings color_settings =
      GetColorSettingsAsImageDataColorSettings();
  if (!buffer || isContextLost()) {
    ImageData* result =
        ImageData::Create(image_data_rect.Size(), &color_settings);
    if (!result)
      exception_state.ThrowRangeError("Out of memory at ImageData creation");
    return result;
  }

  WTF::ArrayBufferContents contents;
  bool is_gpu_readback_invoked = false;
  if (!buffer->GetImageData(image_data_rect, contents,
                            &is_gpu_readback_invoked)) {
    exception_state.ThrowRangeError("Out of memory at ImageData creation");
    return nullptr;
  }

  if (is_gpu_readback_invoked) {
    DidInvokeGPUReadbackInCurrentFrame();
  }

  NeedsFinalizeFrame();

  if (PixelFormat() != kRGBA8CanvasPixelFormat) {
    ImageDataStorageFormat storage_format =
        ImageData::GetImageDataStorageFormat(color_settings.storageFormat());
    DOMArrayBufferView* array_buffer_view =
        ImageData::ConvertPixelsFromCanvasPixelFormatToImageDataStorageFormat(
            contents, PixelFormat(), storage_format);
    return ImageData::Create(image_data_rect.Size(),
                             NotShared<DOMArrayBufferView>(array_buffer_view),
                             &color_settings);
  }
  DOMArrayBuffer* array_buffer = DOMArrayBuffer::Create(contents);
  return ImageData::Create(
      image_data_rect.Size(),
      NotShared<DOMUint8ClampedArray>(DOMUint8ClampedArray::Create(
          array_buffer, 0, array_buffer->ByteLength())),
      &color_settings);
}
