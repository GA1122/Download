payments::FullCardRequest* AutofillManager::GetOrCreateFullCardRequest() {
  if (!full_card_request_) {
    full_card_request_.reset(new payments::FullCardRequest(
        client_, payments_client_.get(), personal_data_));
  }
  return full_card_request_.get();
}
