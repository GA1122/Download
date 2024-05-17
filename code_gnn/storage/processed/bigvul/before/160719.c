base::TimeTicks SanitizeNavigationTiming(
    const base::TimeTicks& browser_navigation_start,
    const base::TimeTicks& renderer_navigation_start) {
  DCHECK(!browser_navigation_start.is_null());
  return std::min(browser_navigation_start, renderer_navigation_start);
}
