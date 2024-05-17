BuildObjectForResourceResponse(const ResourceResponse& response,
                               Resource* cached_resource = nullptr,
                               bool* is_empty = nullptr) {
  if (response.IsNull())
    return nullptr;

  int status;
  String status_text;
  if (response.GetResourceLoadInfo() &&
      response.GetResourceLoadInfo()->http_status_code) {
    status = response.GetResourceLoadInfo()->http_status_code;
    status_text = response.GetResourceLoadInfo()->http_status_text;
  } else {
    status = response.HttpStatusCode();
    status_text = response.HttpStatusText();
  }
  HTTPHeaderMap headers_map;
  if (response.GetResourceLoadInfo() &&
      response.GetResourceLoadInfo()->response_headers.size())
    headers_map = response.GetResourceLoadInfo()->response_headers;
  else
    headers_map = response.HttpHeaderFields();

  int64_t encoded_data_length = response.EncodedDataLength();

  String security_state = protocol::Security::SecurityStateEnum::Unknown;
  switch (response.GetSecurityStyle()) {
    case ResourceResponse::kSecurityStyleUnknown:
      security_state = protocol::Security::SecurityStateEnum::Unknown;
      break;
    case ResourceResponse::kSecurityStyleUnauthenticated:
      security_state = protocol::Security::SecurityStateEnum::Neutral;
      break;
    case ResourceResponse::kSecurityStyleAuthenticationBroken:
      security_state = protocol::Security::SecurityStateEnum::Insecure;
      break;
    case ResourceResponse::kSecurityStyleAuthenticated:
      security_state = protocol::Security::SecurityStateEnum::Secure;
      break;
  }

  String mime_type = response.MimeType();
  if (mime_type.IsEmpty() && cached_resource)
    mime_type = cached_resource->GetResponse().MimeType();

  if (is_empty)
    *is_empty = !status && mime_type.IsEmpty() && !headers_map.size();

  std::unique_ptr<protocol::Network::Response> response_object =
      protocol::Network::Response::create()
          .setUrl(UrlWithoutFragment(response.Url()).GetString())
          .setStatus(status)
          .setStatusText(status_text)
          .setHeaders(BuildObjectForHeaders(headers_map))
          .setMimeType(mime_type)
          .setConnectionReused(response.ConnectionReused())
          .setConnectionId(response.ConnectionID())
          .setEncodedDataLength(encoded_data_length)
          .setSecurityState(security_state)
          .build();

  response_object->setFromDiskCache(response.WasCached());
  response_object->setFromServiceWorker(response.WasFetchedViaServiceWorker());
  if (response.GetResourceLoadTiming())
    response_object->setTiming(
        BuildObjectForTiming(*response.GetResourceLoadTiming()));

  if (response.GetResourceLoadInfo()) {
    if (!response.GetResourceLoadInfo()->response_headers_text.IsEmpty())
      response_object->setHeadersText(
          response.GetResourceLoadInfo()->response_headers_text);
    if (response.GetResourceLoadInfo()->request_headers.size())
      response_object->setRequestHeaders(BuildObjectForHeaders(
          response.GetResourceLoadInfo()->request_headers));
    if (!response.GetResourceLoadInfo()->request_headers_text.IsEmpty())
      response_object->setRequestHeadersText(
          response.GetResourceLoadInfo()->request_headers_text);
  }

  String remote_ip_address = response.RemoteIPAddress();
  if (!remote_ip_address.IsEmpty()) {
    response_object->setRemoteIPAddress(remote_ip_address);
    response_object->setRemotePort(response.RemotePort());
  }

  String protocol;
  if (response.GetResourceLoadInfo())
    protocol = response.GetResourceLoadInfo()->npn_negotiated_protocol;
  if (protocol.IsEmpty() || protocol == "unknown") {
    if (response.WasFetchedViaSPDY()) {
      protocol = "spdy";
    } else if (response.IsHTTP()) {
      protocol = "http";
      if (response.HttpVersion() ==
          ResourceResponse::HTTPVersion::kHTTPVersion_0_9)
        protocol = "http/0.9";
      else if (response.HttpVersion() ==
               ResourceResponse::HTTPVersion::kHTTPVersion_1_0)
        protocol = "http/1.0";
      else if (response.HttpVersion() ==
               ResourceResponse::HTTPVersion::kHTTPVersion_1_1)
        protocol = "http/1.1";
    } else {
      protocol = response.Url().Protocol();
    }
  }
  response_object->setProtocol(protocol);

  if (response.GetSecurityStyle() != ResourceResponse::kSecurityStyleUnknown &&
      response.GetSecurityStyle() !=
          ResourceResponse::kSecurityStyleUnauthenticated) {
    const ResourceResponse::SecurityDetails* response_security_details =
        response.GetSecurityDetails();

    std::unique_ptr<protocol::Array<String>> san_list =
        protocol::Array<String>::create();
    for (auto const& san : response_security_details->san_list)
      san_list->addItem(san);

    std::unique_ptr<
        protocol::Array<protocol::Network::SignedCertificateTimestamp>>
        signed_certificate_timestamp_list = protocol::Array<
            protocol::Network::SignedCertificateTimestamp>::create();
    for (auto const& sct : response_security_details->sct_list) {
      std::unique_ptr<protocol::Network::SignedCertificateTimestamp>
          signed_certificate_timestamp =
              protocol::Network::SignedCertificateTimestamp::create()
                  .setStatus(sct.status_)
                  .setOrigin(sct.origin_)
                  .setLogDescription(sct.log_description_)
                  .setLogId(sct.log_id_)
                  .setTimestamp(sct.timestamp_)
                  .setHashAlgorithm(sct.hash_algorithm_)
                  .setSignatureAlgorithm(sct.signature_algorithm_)
                  .setSignatureData(sct.signature_data_)
                  .build();
      signed_certificate_timestamp_list->addItem(
          std::move(signed_certificate_timestamp));
    }

    std::unique_ptr<protocol::Network::SecurityDetails> security_details =
        protocol::Network::SecurityDetails::create()
            .setProtocol(response_security_details->protocol)
            .setKeyExchange(response_security_details->key_exchange)
            .setCipher(response_security_details->cipher)
            .setSubjectName(response_security_details->subject_name)
            .setSanList(std::move(san_list))
            .setIssuer(response_security_details->issuer)
            .setValidFrom(response_security_details->valid_from)
            .setValidTo(response_security_details->valid_to)
            .setCertificateId(0)   
            .setSignedCertificateTimestampList(
                std::move(signed_certificate_timestamp_list))
            .build();
    if (response_security_details->key_exchange_group.length() > 0)
      security_details->setKeyExchangeGroup(
          response_security_details->key_exchange_group);
    if (response_security_details->mac.length() > 0)
      security_details->setMac(response_security_details->mac);

    response_object->setSecurityDetails(std::move(security_details));
  }

  return response_object;
}
