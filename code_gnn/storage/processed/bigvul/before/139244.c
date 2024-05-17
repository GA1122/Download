AudioSystemImpl::~AudioSystemImpl() {
  AudioSystem::ClearInstance(this);
}
