void ServiceWorkerPaymentInstrument::OnCanMakePaymentEventResponded(
    ValidateCanMakePaymentCallback callback,
    bool result) {
  if (base::FeatureList::IsEnabled(
          ::features::kPaymentRequestHasEnrolledInstrument)) {
    can_make_payment_result_ = true;
    has_enrolled_instrument_result_ = result;
  } else {
    can_make_payment_result_ = result;
    has_enrolled_instrument_result_ = result;
  }
  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE,
      base::BindOnce(std::move(callback), this, can_make_payment_result_));
}
