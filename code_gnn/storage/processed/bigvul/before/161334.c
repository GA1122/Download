void NetworkHandler::NavigationFailed(NavigationRequest* navigation_request) {
  if (!enabled_)
    return;

  static int next_id = 0;
  std::string request_id = base::IntToString(base::GetCurrentProcId()) + "." +
                           base::IntToString(++next_id);
  std::string error_string =
      net::ErrorToString(navigation_request->net_error());
  bool cancelled = navigation_request->net_error() == net::Error::ERR_ABORTED;

  std::unique_ptr<DictionaryValue> headers_dict(DictionaryValue::create());
  net::HttpRequestHeaders headers;
  headers.AddHeadersFromString(navigation_request->begin_params()->headers);
  for (net::HttpRequestHeaders::Iterator it(headers); it.GetNext();)
    headers_dict->setString(it.name(), it.value());
  frontend_->RequestWillBeSent(
      request_id, ""  ,
      navigation_request->common_params().url.spec(),
      Network::Request::Create()
          .SetUrl(navigation_request->common_params().url.spec())
          .SetMethod(navigation_request->common_params().method)
          .SetHeaders(Object::fromValue(headers_dict.get(), nullptr))
          .SetInitialPriority(resourcePriority(net::HIGHEST))
          .SetReferrerPolicy(referrerPolicy(
              navigation_request->common_params().referrer.policy))
          .Build(),
      base::TimeTicks::Now().ToInternalValue() /
          static_cast<double>(base::Time::kMicrosecondsPerSecond),
      base::Time::Now().ToDoubleT(),
      Network::Initiator::Create()
          .SetType(Network::Initiator::TypeEnum::Parser)
          .Build(),
      std::unique_ptr<Network::Response>(),
      std::string(Page::ResourceTypeEnum::Document));

  frontend_->LoadingFailed(
      request_id,
      base::TimeTicks::Now().ToInternalValue() /
          static_cast<double>(base::Time::kMicrosecondsPerSecond),
      Page::ResourceTypeEnum::Document, error_string, cancelled);
}
