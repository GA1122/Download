void PageSnapshotTaker::OnSnapshotEntirePageACK(
    bool success,
    const std::vector<unsigned char>& png_data,
    const std::string& error_msg) {
  bool overall_success = success;
  std::string overall_error_msg = error_msg;
  if (success) {
    base::ThreadRestrictions::ScopedAllowIO allow_io;
    int bytes_written = file_util::WriteFile(image_path_,
        reinterpret_cast<const char*>(&png_data[0]), png_data.size());
    overall_success = (bytes_written == static_cast<int>(png_data.size()));
    if (!overall_success)
      overall_error_msg = "could not write snapshot to disk";
  }
  SendMessage(overall_success, overall_error_msg);
}
