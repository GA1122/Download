FakeCrosDisksClient::FakeCrosDisksClient()
    : unmount_call_count_(0),
      unmount_success_(true),
      format_call_count_(0),
      format_success_(true),
      rename_call_count_(0),
      rename_success_(true),
      weak_ptr_factory_(this) {}
