static int PartiallyRuntimeEnabledOverloadedVoidMethodMethodLength() {
  if (RuntimeEnabledFeatures::RuntimeFeature1Enabled()) {
    return 1;
  }
  if (RuntimeEnabledFeatures::RuntimeFeature2Enabled()) {
    return 1;
  }
  return 2;
}
