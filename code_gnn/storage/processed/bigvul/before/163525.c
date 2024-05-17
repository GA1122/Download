void PushMessagingServiceImpl::SubscribeEndWithError(
    const RegisterCallback& callback,
    content::mojom::PushRegistrationStatus status) {
  SubscribeEnd(callback, std::string()  ,
               std::vector<uint8_t>()  ,
               std::vector<uint8_t>()  , status);
}
