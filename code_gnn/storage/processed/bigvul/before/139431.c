static bool ExecuteInsertHorizontalRule(LocalFrame& frame,
                                        Event*,
                                        EditorCommandSource,
                                        const String& value) {
  DCHECK(frame.GetDocument());
  HTMLHRElement* rule = HTMLHRElement::Create(*frame.GetDocument());
  if (!value.IsEmpty())
    rule->SetIdAttribute(AtomicString(value));
  return ExecuteInsertElement(frame, rule);
}
