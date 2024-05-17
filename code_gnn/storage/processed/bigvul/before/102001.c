void PrintingContextCairo::AskUserForSettings(
    gfx::NativeView parent_view,
    int max_pages,
    bool has_selection,
    PrintSettingsCallback* callback) {
#if defined(OS_CHROMEOS)
  callback->Run(OK);
#else
  print_dialog_->ShowDialog(callback);
#endif   
}
