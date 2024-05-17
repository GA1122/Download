void CSSPaintValue::TraceAfterDispatch(blink::Visitor* visitor) {
  visitor->Trace(name_);
  visitor->Trace(generator_);
  visitor->Trace(paint_image_generator_observer_);
  visitor->Trace(parsed_input_arguments_);
  CSSImageGeneratorValue::TraceAfterDispatch(visitor);
}
