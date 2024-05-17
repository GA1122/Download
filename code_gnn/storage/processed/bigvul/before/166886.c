WebResourceTimingInfo Performance::GenerateResourceTiming(
    const SecurityOrigin& destination_origin,
    const ResourceTimingInfo& info,
    ExecutionContext& context_for_use_counter) {
  const ResourceResponse& final_response = info.FinalResponse();
  WebResourceTimingInfo result;
  result.name = info.InitialURL().GetString();
  result.start_time = info.InitialTime();
  result.alpn_negotiated_protocol = final_response.AlpnNegotiatedProtocol();
  result.connection_info = final_response.ConnectionInfoString();
  result.timing = final_response.GetResourceLoadTiming();
  result.finish_time = info.LoadFinishTime();

  result.allow_timing_details = PassesTimingAllowCheck(
      final_response, destination_origin, info.OriginalTimingAllowOrigin(),
      &context_for_use_counter);

  const Vector<ResourceResponse>& redirect_chain = info.RedirectChain();
  if (!redirect_chain.IsEmpty()) {
    result.allow_redirect_details =
        AllowsTimingRedirect(redirect_chain, final_response, destination_origin,
                             &context_for_use_counter);

    if (ResourceLoadTiming* last_chained_timing =
            redirect_chain.back().GetResourceLoadTiming()) {
      result.last_redirect_end_time =
          TimeTicksInSeconds(last_chained_timing->ReceiveHeadersEnd());
    } else {
      result.allow_redirect_details = false;
      result.last_redirect_end_time = 0.0;
    }
    if (!result.allow_redirect_details) {
      if (ResourceLoadTiming* final_timing =
              final_response.GetResourceLoadTiming()) {
        result.start_time = TimeTicksInSeconds(final_timing->RequestTime());
      }
    }
  } else {
    result.allow_redirect_details = false;
    result.last_redirect_end_time = 0.0;
  }

  result.transfer_size = info.TransferSize();
  result.encoded_body_size = final_response.EncodedBodyLength();
  result.decoded_body_size = final_response.DecodedBodyLength();
  result.did_reuse_connection = final_response.ConnectionReused();
  result.allow_negative_values = info.NegativeAllowed();

  if (result.allow_timing_details) {
    result.server_timing = PerformanceServerTiming::ParseServerTiming(info);
  }
  if (!result.server_timing.empty()) {
    UseCounter::Count(&context_for_use_counter,
                      WebFeature::kPerformanceServerTiming);
  }

  return result;
}
