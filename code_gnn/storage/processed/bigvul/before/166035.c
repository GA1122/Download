void RTCPeerConnectionHandler::SetRemoteDescription(
    const blink::WebRTCVoidRequest& request,
    const blink::WebRTCSessionDescription& description) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  TRACE_EVENT0("webrtc", "RTCPeerConnectionHandler::setRemoteDescription");
  std::string sdp = description.Sdp().Utf8();
  std::string type = description.GetType().Utf8();

  if (peer_connection_tracker_) {
    peer_connection_tracker_->TrackSetSessionDescription(
        this, sdp, type, PeerConnectionTracker::SOURCE_REMOTE);
  }

  webrtc::SdpParseError error;
  std::unique_ptr<webrtc::SessionDescriptionInterface> native_desc(
      CreateNativeSessionDescription(sdp, type, &error));
  if (!native_desc) {
    std::string reason_str = "Failed to parse SessionDescription. ";
    reason_str.append(error.line);
    reason_str.append(" ");
    reason_str.append(error.description);
    LOG(ERROR) << reason_str;
    request.RequestFailed(webrtc::RTCError(
        webrtc::RTCErrorType::UNSUPPORTED_OPERATION, std::move(reason_str)));
    if (peer_connection_tracker_) {
      peer_connection_tracker_->TrackSessionDescriptionCallback(
          this, PeerConnectionTracker::ACTION_SET_REMOTE_DESCRIPTION,
          "OnFailure", reason_str);
    }
    return;
  }

  if (!first_remote_description_ && IsOfferOrAnswer(native_desc.get())) {
    first_remote_description_.reset(
        new FirstSessionDescription(native_desc.get()));
    if (first_local_description_) {
      ReportFirstSessionDescriptions(
          *first_local_description_,
          *first_remote_description_);
    }
  }

  scoped_refptr<WebRtcSetDescriptionObserverImpl> content_observer(
      new WebRtcSetDescriptionObserverImpl(
          weak_factory_.GetWeakPtr(), request, peer_connection_tracker_,
          task_runner_, PeerConnectionTracker::ACTION_SET_REMOTE_DESCRIPTION,
          configuration_.sdp_semantics));

  bool surface_receivers_only =
      (configuration_.sdp_semantics == webrtc::SdpSemantics::kPlanB);
  rtc::scoped_refptr<webrtc::SetRemoteDescriptionObserverInterface>
      webrtc_observer(WebRtcSetRemoteDescriptionObserverHandler::Create(
                          task_runner_, signaling_thread(),
                          native_peer_connection_, track_adapter_map_,
                          content_observer, surface_receivers_only)
                          .get());

  signaling_thread()->PostTask(
      FROM_HERE,
      base::BindOnce(
          &RunClosureWithTrace,
          base::Bind(
              static_cast<void (webrtc::PeerConnectionInterface::*)(
                  std::unique_ptr<webrtc::SessionDescriptionInterface>,
                  rtc::scoped_refptr<
                      webrtc::SetRemoteDescriptionObserverInterface>)>(
                  &webrtc::PeerConnectionInterface::SetRemoteDescription),
              native_peer_connection_, base::Passed(&native_desc),
              webrtc_observer),
          "SetRemoteDescription"));
}
