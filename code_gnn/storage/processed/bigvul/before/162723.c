CanvasPattern* BaseRenderingContext2D::createPattern(
    ScriptState* script_state,
    CanvasImageSource* image_source,
    const String& repetition_type,
    ExceptionState& exception_state) {
  if (!image_source) {
    return nullptr;
  }

  Pattern::RepeatMode repeat_mode =
      CanvasPattern::ParseRepetitionType(repetition_type, exception_state);
  if (exception_state.HadException())
    return nullptr;

  SourceImageStatus status;

  FloatSize default_object_size(Width(), Height());
  scoped_refptr<Image> image_for_rendering =
      image_source->GetSourceImageForCanvas(&status, kPreferNoAcceleration,
                                            kSnapshotReasonCreatePattern,
                                            default_object_size);

  switch (status) {
    case kNormalSourceImageStatus:
      break;
    case kZeroSizeCanvasSourceImageStatus:
      exception_state.ThrowDOMException(
          kInvalidStateError,
          String::Format("The canvas %s is 0.",
                         image_source->ElementSize(default_object_size).Width()
                             ? "height"
                             : "width"));
      return nullptr;
    case kUndecodableSourceImageStatus:
      exception_state.ThrowDOMException(
          kInvalidStateError, "Source image is in the 'broken' state.");
      return nullptr;
    case kInvalidSourceImageStatus:
      image_for_rendering = Image::NullImage();
      break;
    case kIncompleteSourceImageStatus:
      return nullptr;
    default:
      NOTREACHED();
      return nullptr;
  }
  DCHECK(image_for_rendering);

  bool origin_clean =
      !WouldTaintOrigin(image_source, ExecutionContext::From(script_state));

  return CanvasPattern::Create(std::move(image_for_rendering), repeat_mode,
                               origin_clean);
}
