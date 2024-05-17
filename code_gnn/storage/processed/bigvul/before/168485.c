  void CheckHeaderFails(const char* header_field, const char* header_value) {
    WebURLRequest request(ToKURL("http://www.test.com/success.html"));
    request.SetFetchRequestMode(network::mojom::FetchRequestMode::kSameOrigin);
    request.SetFetchCredentialsMode(
        network::mojom::FetchCredentialsMode::kOmit);
    if (EqualIgnoringASCIICase(WebString::FromUTF8(header_field), "referer")) {
      request.SetHTTPReferrer(WebString::FromUTF8(header_value),
                              kWebReferrerPolicyDefault);
    } else {
      request.SetHTTPHeaderField(WebString::FromUTF8(header_field),
                                 WebString::FromUTF8(header_value));
    }

    WebAssociatedURLLoaderOptions options;
    options.untrusted_http = true;
    CheckFails(request, options);
  }
