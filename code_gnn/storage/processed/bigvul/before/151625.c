ChromePaymentRequestDelegate::ChromePaymentRequestDelegate(
    content::WebContents* web_contents)
    : dialog_(nullptr),
      web_contents_(web_contents),
      address_normalizer_(
          GetAddressInputSource(
              GetPersonalDataManager()->GetURLRequestContextGetter()),
          GetAddressInputStorage()) {}
