std::string Backtrace::FormatFrameData(size_t frame_num) {
 if (frame_num >= frames_.size()) {
 return "";
 }
 return FormatFrameData(&frames_[frame_num]);
}
