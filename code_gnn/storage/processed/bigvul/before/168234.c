bool BrowserView::CanActivate() const {
  app_modal::AppModalDialogQueue* queue =
      app_modal::AppModalDialogQueue::GetInstance();
  if (!queue->active_dialog() || !queue->active_dialog()->native_dialog() ||
      !queue->active_dialog()->native_dialog()->IsShowing()) {
    return true;
  }

#if defined(USE_AURA) && defined(OS_CHROMEOS)
  queue->ActivateModalDialog();
#else
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(&BrowserView::ActivateAppModalDialog,
                                activate_modal_dialog_factory_.GetWeakPtr()));
#endif
  return false;
}
