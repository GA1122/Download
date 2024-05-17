void PaymentRequest::CanMakePaymentCallback(bool can_make_payment) {
  if (!spec_ || CanMakePaymentQueryFactory::GetInstance()
                    ->GetForContext(web_contents_->GetBrowserContext())
                    ->CanQuery(top_level_origin_, frame_origin_,
                               spec_->stringified_method_data())) {
    RespondToCanMakePaymentQuery(can_make_payment, false);
  } else if (OriginSecurityChecker::IsOriginLocalhostOrFile(frame_origin_)) {
    RespondToCanMakePaymentQuery(can_make_payment, true);
  } else {
    client_->OnCanMakePayment(
        mojom::CanMakePaymentQueryResult::QUERY_QUOTA_EXCEEDED);
  }

  if (observer_for_testing_)
    observer_for_testing_->OnCanMakePaymentReturned();
}
