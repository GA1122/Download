bool ShallAttemptTpmOwnership() {
#if defined(GOOGLE_CHROME_BUILD)
  return StartupUtils::IsEulaAccepted();
#else
  return false;
#endif
}
