void HTMLMediaElement::AudioClientImpl::SetFormat(uint32_t number_of_channels,
                                                  float sample_rate) {
  if (client_)
    client_->SetFormat(number_of_channels, sample_rate);
}
