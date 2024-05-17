bool JankTimeBomb::IsEnabled() const {
  version_info::Channel channel = chrome::GetChannel();
  return channel == version_info::Channel::UNKNOWN ||
      channel == version_info::Channel::CANARY ||
      channel == version_info::Channel::DEV;
}
