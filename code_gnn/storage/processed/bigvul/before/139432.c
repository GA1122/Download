static bool ExecuteInsertImage(LocalFrame& frame,
                               Event*,
                               EditorCommandSource,
                               const String& value) {
  DCHECK(frame.GetDocument());
  HTMLImageElement* image = HTMLImageElement::Create(*frame.GetDocument());
  if (!value.IsEmpty())
    image->SetSrc(value);
  return ExecuteInsertElement(frame, image);
}
