void PaymentRequestState::CanMakePayment(bool legacy_mode,
                                         StatusCallback callback) {
  if (!get_all_instruments_finished_) {
    DCHECK(!can_make_payment_callback_);
    can_make_payment_callback_ = std::move(callback);
    can_make_payment_legacy_mode_ = legacy_mode;
    return;
  }

  base::ThreadTaskRunnerHandle::Get()->PostTask(
      FROM_HERE, base::BindOnce(&PaymentRequestState::CheckCanMakePayment,
                                weak_ptr_factory_.GetWeakPtr(), legacy_mode,
                                std::move(callback)));
}
