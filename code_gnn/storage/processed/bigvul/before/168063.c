void AutofillManager::OnDidGetRealPan(AutofillClient::PaymentsRpcResult result,
                                      const std::string& real_pan) {
  DCHECK(full_card_request_);
  full_card_request_->OnDidGetRealPan(result, real_pan);
}
