int SaveCardBubbleControllerImpl::GetCvcImageResourceId() const {
  return card_.network() == kAmericanExpressCard ? IDR_CREDIT_CARD_CVC_HINT_AMEX
                                                 : IDR_CREDIT_CARD_CVC_HINT;
}
