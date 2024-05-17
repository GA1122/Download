NOINLINE void CrashIntentionally() {
  volatile int* zero = NULL;
  *zero = 0;
}
