DevToolsURLRequestInterceptor::InterceptionStage ToInterceptorStage(
    const protocol::Network::InterceptionStage& interceptor_stage) {
  if (interceptor_stage == protocol::Network::InterceptionStageEnum::Request)
    return DevToolsURLRequestInterceptor::REQUEST;
  if (interceptor_stage ==
      protocol::Network::InterceptionStageEnum::HeadersReceived)
    return DevToolsURLRequestInterceptor::RESPONSE;
  NOTREACHED();
  return DevToolsURLRequestInterceptor::REQUEST;
}
