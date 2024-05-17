  ProactiveTabFreezeAndDiscardParams GetTestProactiveDiscardParams() {
    ProactiveTabFreezeAndDiscardParams params = {};
    params.should_proactively_discard = true;
    params.low_occluded_timeout = kLowOccludedTimeout;
    params.moderate_occluded_timeout = kModerateOccludedTimeout;
    params.high_occluded_timeout = kHighOccludedTimeout;
    params.low_loaded_tab_count = kLowLoadedTabCount;
    params.moderate_loaded_tab_count = kModerateLoadedTabCount;
    params.high_loaded_tab_count = kHighLoadedTabCount;
    params.freeze_timeout = kFreezeTimeout;
    return params;
  }
