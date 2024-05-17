blink::WebNavigationTimings BuildNavigationTimings(
    base::TimeTicks navigation_start,
    const NavigationTiming& browser_navigation_timings,
    base::TimeTicks input_start) {
  blink::WebNavigationTimings renderer_navigation_timings;

  DCHECK(!navigation_start.is_null());
  renderer_navigation_timings.navigation_start =
      std::min(navigation_start, base::TimeTicks::Now());

  renderer_navigation_timings.redirect_start =
      browser_navigation_timings.redirect_start;
  renderer_navigation_timings.redirect_end =
      browser_navigation_timings.redirect_end;
  renderer_navigation_timings.fetch_start =
      browser_navigation_timings.fetch_start;

  renderer_navigation_timings.input_start = input_start;

  return renderer_navigation_timings;
}
