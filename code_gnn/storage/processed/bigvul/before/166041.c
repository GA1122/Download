  WebRtcSetDescriptionObserverImpl(
      base::WeakPtr<RTCPeerConnectionHandler> handler,
      blink::WebRTCVoidRequest web_request,
      base::WeakPtr<PeerConnectionTracker> tracker,
      scoped_refptr<base::SingleThreadTaskRunner> task_runner,
      PeerConnectionTracker::Action action,
      webrtc::SdpSemantics sdp_semantics)
      : handler_(handler),
        main_thread_(task_runner),
        web_request_(web_request),
        tracker_(tracker),
        action_(action),
        sdp_semantics_(sdp_semantics) {}
