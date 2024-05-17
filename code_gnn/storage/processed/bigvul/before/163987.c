void PaymentRequestState::SetSelectedContactProfile(
    autofill::AutofillProfile* profile) {
  selected_contact_profile_ = profile;

  invalid_contact_profile_ = nullptr;

  UpdateIsReadyToPayAndNotifyObservers();

  if (IsPaymentAppInvoked()) {
    delegate_->OnPayerInfoSelected(
        response_helper_->GeneratePayerDetail(profile));
  }
}
