static bool IsAudible(const AudioBus* rendered_data) {
  float energy = 0;

  unsigned data_size = rendered_data->length();
  for (unsigned k = 0; k < rendered_data->NumberOfChannels(); ++k) {
    const float* data = rendered_data->Channel(k)->Data();
    float channel_energy;
    VectorMath::Vsvesq(data, 1, &channel_energy, data_size);
    energy += channel_energy;
  }

  return energy > 0;
}
