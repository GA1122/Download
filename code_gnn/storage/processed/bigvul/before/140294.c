EditingBehavior Editor::behavior() const {
  if (!frame().settings())
    return EditingBehavior(EditingMacBehavior);

  return EditingBehavior(frame().settings()->getEditingBehaviorType());
}
