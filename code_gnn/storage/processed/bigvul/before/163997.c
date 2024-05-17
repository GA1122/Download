base::string16 ServiceWorkerPaymentInstrument::GetSublabel() const {
  if (needs_installation_) {
    DCHECK(GURL(installable_web_app_info_->sw_scope).is_valid());
    return base::UTF8ToUTF16(
        url::Origin::Create(GURL(installable_web_app_info_->sw_scope)).host());
  }
  return base::UTF8ToUTF16(
      url::Origin::Create(stored_payment_app_info_->scope).host());
}
