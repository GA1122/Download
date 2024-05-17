 void PaymentRequestState::RecordUseStats() {
   if (spec_->request_shipping()) {
     DCHECK(selected_shipping_profile_);
    personal_data_manager_->RecordUseOf(*selected_shipping_profile_);
  }

  if (spec_->request_payer_name() || spec_->request_payer_email() ||
      spec_->request_payer_phone()) {
    DCHECK(selected_contact_profile_);

    if (!spec_->request_shipping() || (selected_shipping_profile_->guid() !=
                                       selected_contact_profile_->guid())) {
      personal_data_manager_->RecordUseOf(*selected_contact_profile_);
    }
  }

  selected_instrument_->RecordUse();
}
