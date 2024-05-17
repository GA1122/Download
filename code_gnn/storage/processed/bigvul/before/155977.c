void DiceTurnSyncOnHelper::ShowSyncConfirmationUI() {
  delegate_->ShowSyncConfirmation(
      base::BindOnce(&DiceTurnSyncOnHelper::FinishSyncSetupAndDelete,
                     weak_pointer_factory_.GetWeakPtr()));
}
