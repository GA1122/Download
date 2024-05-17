  static bool MapStringToOpenType(
      base::StringPiece value,
      file_tasks::FileTasksObserver::OpenType* open_type) {
    using OpenType = file_tasks::FileTasksObserver::OpenType;
    if (value == "launch") {
      *open_type = OpenType::kLaunch;
    } else if (value == "open") {
      *open_type = OpenType::kOpen;
    } else if (value == "saveAs") {
      *open_type = OpenType::kSaveAs;
    } else if (value == "download") {
      *open_type = OpenType::kDownload;
    } else {
      return false;
    }
    return true;
  }
