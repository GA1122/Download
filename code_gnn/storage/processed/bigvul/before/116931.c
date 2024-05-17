TabLoader::TabLoader(base::TimeTicks restore_started)
    : force_load_delay_(kInitialDelayTimerMS),
      loading_(false),
      got_first_paint_(false),
      tab_count_(0),
      restore_started_(restore_started) {
}
