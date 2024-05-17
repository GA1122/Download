void GetSystemSlotOnIOThread(
    base::Callback<void(crypto::ScopedPK11Slot)> callback_ui_thread) {
  auto callback =
      base::BindRepeating(&GotSystemSlotOnIOThread, callback_ui_thread);
  crypto::ScopedPK11Slot system_nss_slot =
      crypto::GetSystemNSSKeySlot(callback);
  if (system_nss_slot) {
    callback.Run(std::move(system_nss_slot));
  }
}
