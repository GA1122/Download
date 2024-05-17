Range* Editor::FindRangeOfString(const String& target,
                                 const EphemeralRangeInFlatTree& reference,
                                 FindOptions options) {
  return FindRangeOfStringAlgorithm<EditingInFlatTreeStrategy>(
      *GetFrame().GetDocument(), target, reference, options);
}
