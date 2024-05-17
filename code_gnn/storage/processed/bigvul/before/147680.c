static int PartiallyRuntimeEnabledOverloadedVoidMethodMethodMaxArg() {
  if (RuntimeEnabledFeatures::RuntimeFeature3Enabled()) {
    return 3;
  }
  return 2;
}
