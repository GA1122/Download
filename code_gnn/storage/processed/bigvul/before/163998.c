void ServiceWorkerPaymentInstrument::InvokePaymentApp(Delegate* delegate) {
  delegate_ = delegate;

  if (needs_installation_) {
    content::PaymentAppProvider::GetInstance()->InstallAndInvokePaymentApp(
        web_contents_, CreatePaymentRequestEventData(),
        installable_web_app_info_->name,
        installable_web_app_info_->icon == nullptr
            ? SkBitmap()
            : *(installable_web_app_info_->icon),
        installable_web_app_info_->sw_js_url,
        installable_web_app_info_->sw_scope,
        installable_web_app_info_->sw_use_cache, installable_enabled_method_,
        base::BindOnce(&ServiceWorkerPaymentInstrument::OnPaymentAppInvoked,
                       weak_ptr_factory_.GetWeakPtr()));
  } else {
    content::PaymentAppProvider::GetInstance()->InvokePaymentApp(
        browser_context_, stored_payment_app_info_->registration_id,
        CreatePaymentRequestEventData(),
        base::BindOnce(&ServiceWorkerPaymentInstrument::OnPaymentAppInvoked,
                       weak_ptr_factory_.GetWeakPtr()));
  }

   payment_request_delegate_->ShowProcessingSpinner();
 }
