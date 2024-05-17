PeriodicWave* BaseAudioContext::createPeriodicWave(
    const Vector<float>& real,
    const Vector<float>& imag,
    const PeriodicWaveConstraints* options,
    ExceptionState& exception_state) {
  DCHECK(IsMainThread());

  bool disable = options->disableNormalization();

  return PeriodicWave::Create(*this, real, imag, disable, exception_state);
}
