bool RTCPeerConnectionHandler::Initialize(
    const webrtc::PeerConnectionInterface::RTCConfiguration&
        server_configuration,
    const blink::WebMediaConstraints& options) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  DCHECK(frame_);

  CHECK(!initialize_called_);
  initialize_called_ = true;

  peer_connection_tracker_ =
      RenderThreadImpl::current()->peer_connection_tracker()->AsWeakPtr();

  configuration_ = server_configuration;

  configuration_.set_prerenderer_smoothing(
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableRTCSmoothnessAlgorithm));

  configuration_.set_experiment_cpu_load_estimator(
      base::FeatureList::IsEnabled(media::kNewEncodeCpuLoadEstimator));

  configuration_.crypto_options = webrtc::CryptoOptions{};
  configuration_.crypto_options->srtp.enable_gcm_crypto_suites =
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableWebRtcSrtpAesGcm);
  configuration_.crypto_options->srtp.enable_encrypted_rtp_header_extensions =
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableWebRtcSrtpEncryptedHeaders);

  CopyConstraintsIntoRtcConfiguration(options, &configuration_);

  peer_connection_observer_ =
      new Observer(weak_factory_.GetWeakPtr(), task_runner_);
  native_peer_connection_ = dependency_factory_->CreatePeerConnection(
      configuration_, frame_, peer_connection_observer_.get());

  if (!native_peer_connection_.get()) {
    LOG(ERROR) << "Failed to initialize native PeerConnection.";
    return false;
  }

  if (peer_connection_tracker_) {
    peer_connection_tracker_->RegisterPeerConnection(this, configuration_,
                                                     options, frame_);
  }

  return true;
}
