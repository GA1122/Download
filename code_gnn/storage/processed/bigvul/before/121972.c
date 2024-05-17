int GetAppListIconIndex() {
  const int kAppListIconIndex = 5;
  const int kAppListIconIndexSxS = 6;
  const int kAppListIconIndexChromium = 1;
#if defined(GOOGLE_CHROME_BUILD)
  if (InstallUtil::IsChromeSxSProcess())
    return kAppListIconIndexSxS;
  return kAppListIconIndex;
#else
  return kAppListIconIndexChromium;
#endif
}
