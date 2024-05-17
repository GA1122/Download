void ServiceWorkerPaymentInstrument::ValidateCanMakePayment(
    ValidateCanMakePaymentCallback callback) {
  if (needs_installation_) {
    OnCanMakePaymentEventSkipped(std::move(callback));
    return;
  }

  if (payment_request_delegate_->IsIncognito()) {
    OnCanMakePaymentEventSkipped(std::move(callback));
    return;
  }

  if (!stored_payment_app_info_->has_explicitly_verified_methods) {
    OnCanMakePaymentEventSkipped(std::move(callback));
    return;
  }

  mojom::CanMakePaymentEventDataPtr event_data =
      CreateCanMakePaymentEventData();
  if (event_data.is_null()) {
    OnCanMakePaymentEventSkipped(std::move(callback));
    return;
  }

  content::PaymentAppProvider::GetInstance()->CanMakePayment(
      browser_context_, stored_payment_app_info_->registration_id,
      std::move(event_data),
      base::BindOnce(
          &ServiceWorkerPaymentInstrument::OnCanMakePaymentEventResponded,
          weak_ptr_factory_.GetWeakPtr(), std::move(callback)));
}
