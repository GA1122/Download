void NetworkHandler::NavigationPreloadRequestSent(
    const std::string& request_id,
    const network::ResourceRequest& request) {
  if (!enabled_)
    return;
  std::unique_ptr<DictionaryValue> headers_dict(DictionaryValue::create());
  for (net::HttpRequestHeaders::Iterator it(request.headers); it.GetNext();)
    headers_dict->setString(it.name(), it.value());
  frontend_->RequestWillBeSent(
      request_id, ""  , request.url.spec(),
      Network::Request::Create()
          .SetUrl(request.url.spec())
          .SetMethod(request.method)
          .SetHeaders(Object::fromValue(headers_dict.get(), nullptr))
          .SetInitialPriority(resourcePriority(request.priority))
          .SetReferrerPolicy(referrerPolicy(request.referrer_policy))
          .Build(),
      base::TimeTicks::Now().ToInternalValue() /
          static_cast<double>(base::Time::kMicrosecondsPerSecond),
      base::Time::Now().ToDoubleT(),
      Network::Initiator::Create()
          .SetType(Network::Initiator::TypeEnum::Preload)
          .Build(),
      std::unique_ptr<Network::Response>(),
      std::string(Page::ResourceTypeEnum::Other));
}
