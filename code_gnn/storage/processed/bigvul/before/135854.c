CueTimeline::CueTimeline(HTMLMediaElement& media_element)
    : media_element_(&media_element),
      last_update_time_(-1),
      ignore_update_(0) {}
