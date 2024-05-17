  IndicatorType indicator_type() {
    DCHECK(window_selector());
    return window_selector()
        ->split_view_overview_overlay()
        ->current_indicator_type();
  }
