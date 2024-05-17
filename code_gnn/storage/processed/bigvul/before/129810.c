SystemClipboard::SystemClipboard() {
  Platform::Current()->GetInterfaceProvider()->GetInterface(
      mojo::MakeRequest(&clipboard_));
}
