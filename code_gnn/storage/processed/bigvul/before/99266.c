URLRequestContext* ResourceMessageFilter::GetRequestContext(
    uint32 request_id,
    const ViewHostMsg_Resource_Request& request_data) {
  URLRequestContextGetter* request_context = request_context_;
  if (request_data.resource_type == ResourceType::MEDIA)
    request_context = media_request_context_;
  return request_context->GetURLRequestContext();
}
