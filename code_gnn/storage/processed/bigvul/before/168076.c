void AutofillManager::OnUnmaskVerificationResult(
    AutofillClient::PaymentsRpcResult result) {
  client_->OnUnmaskVerificationResult(result);
}
