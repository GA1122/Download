UINT32 GKI_get_os_tick_count(void) {
 struct timespec timespec;
  clock_gettime(CLOCK_BOOTTIME, &timespec);
 return (timespec.tv_sec * 1000) + (timespec.tv_nsec / 1000000);
}
