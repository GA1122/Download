Range* Editor::findRangeOfString(const String& target,
                                 const EphemeralRange& reference,
                                 FindOptions options) {
  return findRangeOfStringAlgorithm<EditingStrategy>(
      *frame().document(), target, reference, options);
}
