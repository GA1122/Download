bool Editor::TryDHTMLPaste(PasteMode paste_mode) {
  return !DispatchCPPEvent(EventTypeNames::paste, kDataTransferReadable,
                           paste_mode);
}
