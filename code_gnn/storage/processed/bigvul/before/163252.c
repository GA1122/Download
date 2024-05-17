void BrowserMainLoop::CreateAudioManager() {
  DCHECK(!audio_manager_);

  audio_manager_ = GetContentClient()->browser()->CreateAudioManager(
      MediaInternals::GetInstance());
  if (!audio_manager_) {
    audio_manager_ =
        media::AudioManager::Create(std::make_unique<media::AudioThreadImpl>(),
                                    MediaInternals::GetInstance());
  }
  CHECK(audio_manager_);
  audio_system_ = media::AudioSystem::CreateInstance();
  CHECK(audio_system_);
}
