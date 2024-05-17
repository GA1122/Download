void RTCPeerConnectionHandler::SetLocalDescription(
    const blink::WebRTCVoidRequest& request,
    const blink::WebRTCSessionDescription& description) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  TRACE_EVENT0("webrtc", "RTCPeerConnectionHandler::setLocalDescription");

  std::string sdp = description.Sdp().Utf8();
  std::string type = description.GetType().Utf8();

  if (peer_connection_tracker_) {
    peer_connection_tracker_->TrackSetSessionDescription(
        this, sdp, type, PeerConnectionTracker::SOURCE_LOCAL);
  }

  webrtc::SdpParseError error;
  webrtc::SessionDescriptionInterface* native_desc =
      CreateNativeSessionDescription(sdp, type, &error);
  if (!native_desc) {
    std::string reason_str = "Failed to parse SessionDescription. ";
    reason_str.append(error.line);
    reason_str.append(" ");
    reason_str.append(error.description);
    LOG(ERROR) << reason_str;
    request.RequestFailed(webrtc::RTCError(webrtc::RTCErrorType::INTERNAL_ERROR,
                                           std::move(reason_str)));
    if (peer_connection_tracker_) {
      peer_connection_tracker_->TrackSessionDescriptionCallback(
          this, PeerConnectionTracker::ACTION_SET_LOCAL_DESCRIPTION,
          "OnFailure", reason_str);
    }
    return;
  }

  if (!first_local_description_ && IsOfferOrAnswer(native_desc)) {
    first_local_description_.reset(new FirstSessionDescription(native_desc));
    if (first_remote_description_) {
      ReportFirstSessionDescriptions(
          *first_local_description_,
          *first_remote_description_);
    }
  }

  scoped_refptr<WebRtcSetDescriptionObserverImpl> content_observer(
      new WebRtcSetDescriptionObserverImpl(
          weak_factory_.GetWeakPtr(), request, peer_connection_tracker_,
          task_runner_, PeerConnectionTracker::ACTION_SET_LOCAL_DESCRIPTION,
          configuration_.sdp_semantics));

  bool surface_receivers_only =
      (configuration_.sdp_semantics == webrtc::SdpSemantics::kPlanB);
  scoped_refptr<webrtc::SetSessionDescriptionObserver> webrtc_observer(
      WebRtcSetLocalDescriptionObserverHandler::Create(
          task_runner_, signaling_thread(), native_peer_connection_,
          track_adapter_map_, content_observer, surface_receivers_only)
          .get());

  signaling_thread()->PostTask(
      FROM_HERE,
      base::BindOnce(
          &RunClosureWithTrace,
          base::Bind(&webrtc::PeerConnectionInterface::SetLocalDescription,
                     native_peer_connection_,
                     base::RetainedRef(webrtc_observer),
                     base::Unretained(native_desc)),
          "SetLocalDescription"));
}
