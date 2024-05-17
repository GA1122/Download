inline bool IsArchitectureArm() {
#if defined(__arm__) || defined(__aarch64__)
  return true;
#else
  return false;
#endif
}
