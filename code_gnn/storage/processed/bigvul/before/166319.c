int SpeechRecognitionManagerImpl::CreateSession(
    const SpeechRecognitionSessionConfig& config) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  const int session_id = GetNextSessionID();
  DCHECK(!SessionExists(session_id));

  auto session = std::make_unique<Session>();
  session->id = session_id;
  session->config = config;
  session->context = config.initial_context;

#if !defined(OS_ANDROID)

  SpeechRecognitionEngine::Config remote_engine_config;
  remote_engine_config.language = config.language;
  remote_engine_config.grammars = config.grammars;
  remote_engine_config.audio_sample_rate =
      SpeechRecognizerImpl::kAudioSampleRate;
  remote_engine_config.audio_num_bits_per_sample =
      SpeechRecognizerImpl::kNumBitsPerAudioSample;
  remote_engine_config.filter_profanities = config.filter_profanities;
  remote_engine_config.continuous = config.continuous;
  remote_engine_config.interim_results = config.interim_results;
  remote_engine_config.max_hypotheses = config.max_hypotheses;
  remote_engine_config.origin_url = config.origin.Serialize();
  remote_engine_config.auth_token = config.auth_token;
  remote_engine_config.auth_scope = config.auth_scope;
  remote_engine_config.preamble = config.preamble;

  SpeechRecognitionEngine* google_remote_engine = new SpeechRecognitionEngine(
      config.shared_url_loader_factory, config.accept_language);
  google_remote_engine->SetConfig(remote_engine_config);

  session->recognizer = new SpeechRecognizerImpl(
      this, audio_system_, session_id, config.continuous,
      config.interim_results, google_remote_engine);
#else
  session->recognizer = new SpeechRecognizerImplAndroid(this, session_id);
#endif

  sessions_[session_id] = std::move(session);

  base::CreateSingleThreadTaskRunnerWithTraits({BrowserThread::UI})
      ->PostTask(
          FROM_HERE,
          base::BindOnce(&SpeechRecognitionManagerImpl::FrameDeletionObserver::
                             CreateObserverForSession,
                         base::Unretained(frame_deletion_observer_.get()),
                         config.initial_context.render_process_id,
                         config.initial_context.render_frame_id, session_id));

  return session_id;
}
