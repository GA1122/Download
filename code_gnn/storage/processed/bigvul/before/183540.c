 void Vp9Parser::Reset() {
  stream_ = nullptr;
  bytes_left_ = 0;
  frames_.clear();
  memset(&segmentation_, 0, sizeof(segmentation_));
  memset(&loop_filter_, 0, sizeof(loop_filter_));
// Vp9Parser::Vp9Parser() : stream_(nullptr), size_(0) {
    memset(&ref_slots_, 0, sizeof(ref_slots_));
  }