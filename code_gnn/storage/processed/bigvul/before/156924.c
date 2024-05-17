void DocumentLoader::UpdateNavigationTimings(
    base::TimeTicks navigation_start_time,
    base::TimeTicks redirect_start_time,
    base::TimeTicks redirect_end_time,
    base::TimeTicks fetch_start_time,
    base::TimeTicks input_start_time) {
  if (!input_start_time.is_null()) {
    GetTiming().SetInputStart(input_start_time);
  }

  if (navigation_start_time.is_null()) {
    GetTiming().SetNavigationStart(CurrentTimeTicks());
    return;
  }

  GetTiming().SetNavigationStart(navigation_start_time);
  if (!redirect_start_time.is_null()) {
    GetTiming().SetRedirectStart(redirect_start_time);
    GetTiming().SetRedirectEnd(redirect_end_time);
  }
  if (!fetch_start_time.is_null()) {
    DCHECK(!navigation_start_time.is_null());
    GetTiming().SetFetchStart(fetch_start_time);
  }
}
