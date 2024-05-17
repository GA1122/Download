BookmarksIOFunction::~BookmarksIOFunction() {
  if (select_file_dialog_.get())
    select_file_dialog_->ListenerDestroyed();
}
