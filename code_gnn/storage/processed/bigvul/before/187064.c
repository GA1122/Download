  void PaymentRequest::CanMakePayment() {
//   if (!IsInitialized()) {
//     log_.Error("Attempted canMakePayment without initialization");
//     OnConnectionTerminated();
//     return;
//   }
// 
//    
// 
    if (observer_for_testing_)
      observer_for_testing_->OnCanMakePaymentCalled();
  
   if (!delegate_->GetPrefService()->GetBoolean(kCanMakePaymentEnabled) ||
       !state_) {
     CanMakePaymentCallback( false);
   } else {
     state_->CanMakePayment(
         base::BindOnce(&PaymentRequest::CanMakePaymentCallback,
                        weak_ptr_factory_.GetWeakPtr()));
    }
  }