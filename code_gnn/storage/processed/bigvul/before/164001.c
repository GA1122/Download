bool ServiceWorkerPaymentInstrument::IsValidForCanMakePayment() const {
  DCHECK(can_make_payment_result_);
  if (base::FeatureList::IsEnabled(
          ::features::kPaymentRequestHasEnrolledInstrument)) {
    return has_enrolled_instrument_result_;
  }
  return true;
}
