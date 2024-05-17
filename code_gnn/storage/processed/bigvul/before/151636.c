ChromePaymentRequestDelegate::GetRegionDataLoader() {
  return new autofill::RegionDataLoaderImpl(
      GetAddressInputSource(
          GetPersonalDataManager()->GetURLRequestContextGetter())
          .release(),
      GetAddressInputStorage().release(), GetApplicationLocale());
}
