void HTMLMediaElement::AudioClientImpl::SetFormat(size_t number_of_channels,
                                                  float sample_rate) {
  if (client_)
    client_->SetFormat(number_of_channels, sample_rate);
}
