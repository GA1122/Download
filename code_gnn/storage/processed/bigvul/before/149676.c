base::string16 SaveCardBubbleControllerImpl::GetCvcEnteredByUser() const {
  DCHECK(!cvc_entered_by_user_.empty());
  return cvc_entered_by_user_;
}
