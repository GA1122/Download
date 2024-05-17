void PPB_URLLoader_Impl::didFail(WebURLLoader* loader,
                                 const WebURLError& error) {
  if (error.domain.equals(WebString::fromUTF8(net::kErrorDomain))) {
    switch (error.reason) {
      case net::ERR_ACCESS_DENIED:
      case net::ERR_NETWORK_ACCESS_DENIED:
        done_status_ = PP_ERROR_NOACCESS;
        break;
      default:
        done_status_ = PP_ERROR_FAILED;
        break;
    }
  } else {
    done_status_ = PP_ERROR_NOACCESS;
  }

  RunCallback(done_status_);
}
