Element* Editor::FindEventTargetFrom(const VisibleSelection& selection) const {
  Element* target = AssociatedElementOf(selection.Start());
  if (!target)
    target = GetFrame().GetDocument()->body();

  return target;
}
