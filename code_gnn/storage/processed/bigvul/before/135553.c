Range* Editor::FindRangeOfString(const String& target,
                                 const EphemeralRange& reference,
                                 FindOptions options) {
  return FindRangeOfStringAlgorithm<EditingStrategy>(
      *GetFrame().GetDocument(), target, reference, options);
}
