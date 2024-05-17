void HTMLMediaElement::AudioClientImpl::setFormat(size_t numberOfChannels,
                                                  float sampleRate) {
  if (m_client)
    m_client->setFormat(numberOfChannels, sampleRate);
}
