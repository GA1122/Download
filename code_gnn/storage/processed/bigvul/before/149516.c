PageRequestSummary::PageRequestSummary(const GURL& i_main_frame_url)
    : main_frame_url(i_main_frame_url),
      initial_url(i_main_frame_url),
      first_contentful_paint(base::TimeTicks::Max()) {}
