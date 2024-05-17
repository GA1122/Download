void PPB_URLLoader_Impl::didReceiveResponse(WebURLLoader* loader,
                                            const WebURLResponse& response) {
  SaveResponse(response);

  total_bytes_to_be_received_ = response.expectedContentLength();
  UpdateStatus();

  RunCallback(PP_OK);
}
