void SupervisedUserService::SetDelegate(Delegate* delegate) {
  if (delegate) {
    DCHECK(!delegate_);
  } else {
    SetActive(false);
  }
  delegate_ = delegate;
}
