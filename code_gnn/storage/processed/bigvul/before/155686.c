void AuthenticatorClientPinEntrySheetModel::SetDelegate(Delegate* delegate) {
  DCHECK(!delegate_);
  delegate_ = delegate;
}
