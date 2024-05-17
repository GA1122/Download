void TestingAutomationProvider::CaptureProfilePhoto(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  chromeos::TakePhotoDialog* take_photo_dialog =
      new chromeos::TakePhotoDialog(NULL);

  take_photo_dialog->AddObserver(new PhotoCaptureObserver(
      this, reply_message));

  views::Widget* window = browser::CreateViewsWindow(
      browser->window()->GetNativeHandle(), take_photo_dialog, STYLE_GENERIC);
  window->SetAlwaysOnTop(true);
  window->Show();
}
