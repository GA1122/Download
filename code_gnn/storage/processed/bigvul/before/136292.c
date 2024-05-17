static bool IsPrintingRootLayoutView(const LayoutObject& object) {
  if (!object.IsLayoutView())
    return false;

  const auto& frame = *object.GetFrame();
  if (!frame.GetDocument()->Printing())
    return false;

  const auto* parent_frame = frame.Tree().Parent();
  if (!parent_frame)
    return true;
  if (!parent_frame->IsLocalFrame())
    return true;

  return !ToLocalFrame(parent_frame)->GetDocument()->Printing();
}
