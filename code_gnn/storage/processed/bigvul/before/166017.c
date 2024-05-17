RTCPeerConnectionHandler::RTCPeerConnectionHandler(
    blink::WebRTCPeerConnectionHandlerClient* client,
    PeerConnectionDependencyFactory* dependency_factory,
    scoped_refptr<base::SingleThreadTaskRunner> task_runner)
    : id_(base::ToUpperASCII(base::UnguessableToken::Create().ToString())),
      initialize_called_(false),
      client_(client),
      is_closed_(false),
      dependency_factory_(dependency_factory),
      track_adapter_map_(
          new WebRtcMediaStreamTrackAdapterMap(dependency_factory_,
                                               task_runner)),
      task_runner_(std::move(task_runner)),
      weak_factory_(this) {
  CHECK(client_);

  GetPeerConnectionHandlers()->insert(this);
}
