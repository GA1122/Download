MediaStreamManager::MediaStreamManager(
    media::AudioSystem* audio_system,
    scoped_refptr<base::SingleThreadTaskRunner> audio_task_runner)
    : MediaStreamManager(audio_system, std::move(audio_task_runner), nullptr) {}
