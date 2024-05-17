PageHandler::PageHandler(EmulationHandler* emulation_handler)
    : DevToolsDomainHandler(Page::Metainfo::domainName),
      enabled_(false),
      screencast_enabled_(false),
      screencast_quality_(kDefaultScreenshotQuality),
      screencast_max_width_(-1),
      screencast_max_height_(-1),
      capture_every_nth_frame_(1),
      capture_retry_count_(0),
      has_compositor_frame_metadata_(false),
      session_id_(0),
      frame_counter_(0),
      frames_in_flight_(0),
      host_(nullptr),
      emulation_handler_(emulation_handler),
      weak_factory_(this) {
  DCHECK(emulation_handler_);
}
