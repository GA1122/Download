PaymentRequestState::PaymentRequestState(
    content::WebContents* web_contents,
    const GURL& top_level_origin,
    const GURL& frame_origin,
    PaymentRequestSpec* spec,
    Delegate* delegate,
    const std::string& app_locale,
    autofill::PersonalDataManager* personal_data_manager,
    ContentPaymentRequestDelegate* payment_request_delegate,
    JourneyLogger* journey_logger)
    : is_ready_to_pay_(false),
      get_all_instruments_finished_(true),
      is_waiting_for_merchant_validation_(false),
      app_locale_(app_locale),
      spec_(spec),
      delegate_(delegate),
      personal_data_manager_(personal_data_manager),
      journey_logger_(journey_logger),
      are_requested_methods_supported_(
          !spec_->supported_card_networks().empty()),
      selected_shipping_profile_(nullptr),
      selected_shipping_option_error_profile_(nullptr),
      selected_contact_profile_(nullptr),
      invalid_shipping_profile_(nullptr),
      invalid_contact_profile_(nullptr),
      selected_instrument_(nullptr),
      number_of_pending_sw_payment_instruments_(0),
      payment_request_delegate_(payment_request_delegate),
      profile_comparator_(app_locale, *spec),
      weak_ptr_factory_(this) {
  if (base::FeatureList::IsEnabled(::features::kServiceWorkerPaymentApps)) {
    DCHECK(web_contents);
    get_all_instruments_finished_ = false;
    ServiceWorkerPaymentAppFactory::GetInstance()->GetAllPaymentApps(
        web_contents,
        payment_request_delegate_->GetPaymentManifestWebDataService(),
        spec_->method_data(),
         
        !spec_->supports_basic_card(),
        base::BindOnce(&PaymentRequestState::GetAllPaymentAppsCallback,
                       weak_ptr_factory_.GetWeakPtr(), web_contents,
                       top_level_origin, frame_origin),
        base::BindOnce([]() {
           
        }));
  } else {
    PopulateProfileCache();
    SetDefaultProfileSelections();
  }
  spec_->AddObserver(this);
}
