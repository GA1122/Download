Range* Editor::findRangeOfString(const String& target,
                                 const EphemeralRangeInFlatTree& reference,
                                 FindOptions options) {
  return findRangeOfStringAlgorithm<EditingInFlatTreeStrategy>(
      *frame().document(), target, reference, options);
}
