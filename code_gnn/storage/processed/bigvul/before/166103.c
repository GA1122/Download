void PaymentRequest::OnPayerInfoSelected(mojom::PayerDetailPtr payer_info) {
  client_->OnPayerDetailChange(std::move(payer_info));
}
