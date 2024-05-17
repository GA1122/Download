PeriodicWave* BaseAudioContext::GetPeriodicWave(int type) {
  switch (type) {
    case OscillatorHandler::SINE:
      if (!periodic_wave_sine_)
        periodic_wave_sine_ = PeriodicWave::CreateSine(sampleRate());
      return periodic_wave_sine_;
    case OscillatorHandler::SQUARE:
      if (!periodic_wave_square_)
        periodic_wave_square_ = PeriodicWave::CreateSquare(sampleRate());
      return periodic_wave_square_;
    case OscillatorHandler::SAWTOOTH:
      if (!periodic_wave_sawtooth_)
        periodic_wave_sawtooth_ = PeriodicWave::CreateSawtooth(sampleRate());
      return periodic_wave_sawtooth_;
    case OscillatorHandler::TRIANGLE:
      if (!periodic_wave_triangle_)
        periodic_wave_triangle_ = PeriodicWave::CreateTriangle(sampleRate());
      return periodic_wave_triangle_;
    default:
      NOTREACHED();
      return nullptr;
  }
}
