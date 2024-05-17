void GotSystemSlotOnIOThread(
    base::Callback<void(crypto::ScopedPK11Slot)> callback_ui_thread,
    crypto::ScopedPK11Slot system_slot) {
  content::BrowserThread::PostTask(
      content::BrowserThread::UI, FROM_HERE,
      base::BindOnce(&GotSystemSlotOnUIThread, callback_ui_thread,
                     std::move(system_slot)));
}
