EditingBehavior Editor::Behavior() const {
  if (!GetFrame().GetSettings())
    return EditingBehavior(kEditingMacBehavior);

  return EditingBehavior(GetFrame().GetSettings()->GetEditingBehaviorType());
}
