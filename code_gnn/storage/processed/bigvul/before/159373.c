bool ExtensionApiTest::ExtensionSubtestsAreSkipped() {
#if defined(OS_WIN) && !defined(NDEBUG)
  LOG(WARNING) << "Workaround for 177163, prematurely returning";
  return true;
#else
  return false;
#endif
}
