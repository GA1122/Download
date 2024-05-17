payments::FullCardRequest* AutofillManager::CreateFullCardRequest(
    const base::TimeTicks& form_parsed_timestamp) {
  full_card_request_.reset(new payments::FullCardRequest(
      client_, payments_client_.get(), personal_data_, form_parsed_timestamp));
  return full_card_request_.get();
}
