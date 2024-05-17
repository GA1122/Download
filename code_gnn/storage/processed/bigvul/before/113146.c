void Launcher::ActivateLauncherItem(int index) {
  DCHECK(delegate_.get());
  const ash::LauncherItems& items = model_->items();
  delegate_->ItemClicked(items[index], ui::EF_NONE);
}
