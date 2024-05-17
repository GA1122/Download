GURL PushMessagingServiceImpl::GetEndpoint(bool standard_protocol) const {
  return GURL(standard_protocol ? kPushMessagingPushProtocolEndpoint
                                : kPushMessagingGcmEndpoint);
}
