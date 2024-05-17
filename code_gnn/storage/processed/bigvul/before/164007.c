ServiceWorkerPaymentInstrument::ServiceWorkerPaymentInstrument(
    content::WebContents* web_contents,
    const GURL& top_origin,
    const GURL& frame_origin,
    const PaymentRequestSpec* spec,
    std::unique_ptr<WebAppInstallationInfo> installable_payment_app_info,
    const std::string& enabled_method,
    PaymentRequestDelegate* payment_request_delegate)
    : PaymentInstrument(0, PaymentInstrument::Type::SERVICE_WORKER_APP),
      top_origin_(top_origin),
      frame_origin_(frame_origin),
      spec_(spec),
      delegate_(nullptr),
      payment_request_delegate_(payment_request_delegate),
      can_make_payment_result_(false),
      has_enrolled_instrument_result_(false),
      needs_installation_(true),
      web_contents_(web_contents),
      installable_web_app_info_(std::move(installable_payment_app_info)),
      installable_enabled_method_(enabled_method),
      weak_ptr_factory_(this) {
  DCHECK(web_contents_);
  DCHECK(top_origin_.is_valid());
  DCHECK(frame_origin_.is_valid());
  DCHECK(spec_);

  if (installable_web_app_info_->icon) {
    icon_image_ =
        gfx::ImageSkia::CreateFrom1xBitmap(*(installable_web_app_info_->icon))
            .DeepCopy();
  } else {
    icon_image_ = gfx::ImageSkia::CreateFrom1xBitmap(SkBitmap()).DeepCopy();
  }
}
