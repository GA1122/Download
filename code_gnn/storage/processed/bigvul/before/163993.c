ServiceWorkerPaymentInstrument::CreateCanMakePaymentEventData() {
  std::set<std::string> requested_url_methods;
  for (const auto& method : spec_->payment_method_identifiers_set()) {
    GURL url_method(method);
    if (url_method.is_valid()) {
      requested_url_methods.insert(method);
    }
  }
  std::set<std::string> supported_methods;
  supported_methods.insert(stored_payment_app_info_->enabled_methods.begin(),
                           stored_payment_app_info_->enabled_methods.end());
  std::set<std::string> supported_url_methods =
      base::STLSetIntersection<std::set<std::string>>(requested_url_methods,
                                                      supported_methods);
  if (supported_url_methods.empty())
    return nullptr;

  mojom::CanMakePaymentEventDataPtr event_data =
      mojom::CanMakePaymentEventData::New();

  event_data->top_origin = top_origin_;
  event_data->payment_request_origin = frame_origin_;

  DCHECK(spec_->details().modifiers);
  for (const auto& modifier : *spec_->details().modifiers) {
    if (base::ContainsKey(supported_url_methods,
                          modifier->method_data->supported_method)) {
      event_data->modifiers.emplace_back(modifier.Clone());
    }
  }

  for (const auto& data : spec_->method_data()) {
    if (base::ContainsKey(supported_url_methods, data->supported_method)) {
      event_data->method_data.push_back(data.Clone());
    }
  }

  return event_data;
}
