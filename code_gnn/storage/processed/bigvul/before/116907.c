void TabLoader::ForceLoadTimerFired() {
  force_load_delay_ *= 2;
  LoadNextTab();
}
