void GotSystemSlotOnUIThread(
    base::Callback<void(crypto::ScopedPK11Slot)> callback_ui_thread,
    crypto::ScopedPK11Slot system_slot) {
  callback_ui_thread.Run(std::move(system_slot));
}
