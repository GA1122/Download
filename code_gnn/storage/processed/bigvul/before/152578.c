CommonNavigationParams MakeCommonNavigationParams(
    TimeDelta navigation_start_offset) {
  CommonNavigationParams params;
  params.url = GURL("data:text/html,<div>Page</div>");
  params.navigation_start = base::TimeTicks::Now() + navigation_start_offset;
  params.navigation_type = FrameMsg_Navigate_Type::DIFFERENT_DOCUMENT;
  params.transition = ui::PAGE_TRANSITION_TYPED;
  return params;
}
