bool ServiceWorkerPaymentInstrument::IsValidForModifier(
    const std::string& method,
    bool supported_networks_specified,
    const std::set<std::string>& supported_networks,
    bool supported_types_specified,
    const std::set<autofill::CreditCard::CardType>& supported_types) const {
  if (needs_installation_)
    return installable_enabled_method_ == method;

  if (!base::ContainsValue(stored_payment_app_info_->enabled_methods, method))
    return false;

  if (method != "basic-card")
    return true;

  if (!supported_networks_specified && !supported_types_specified)
    return true;

  if (stored_payment_app_info_->capabilities.empty())
    return false;

  uint32_t i = 0;
  for (; i < stored_payment_app_info_->capabilities.size(); i++) {
    if (supported_networks_specified) {
      std::set<std::string> app_supported_networks;
      for (const auto& network :
           stored_payment_app_info_->capabilities[i].supported_card_networks) {
        app_supported_networks.insert(GetBasicCardNetworkName(
            static_cast<mojom::BasicCardNetwork>(network)));
      }

      if (base::STLSetIntersection<std::set<std::string>>(
              app_supported_networks, supported_networks)
              .empty()) {
        continue;
      }
    }

    if (supported_types_specified) {
      std::set<autofill::CreditCard::CardType> app_supported_types;
      for (const auto& type :
           stored_payment_app_info_->capabilities[i].supported_card_types) {
        app_supported_types.insert(
            GetBasicCardType(static_cast<mojom::BasicCardType>(type)));
      }

      if (base::STLSetIntersection<std::set<autofill::CreditCard::CardType>>(
              app_supported_types, supported_types)
              .empty()) {
        continue;
      }
    }

    break;
  }

  return i < stored_payment_app_info_->capabilities.size();
}
