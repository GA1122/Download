void GamepadProvider::ResetVibrationActuator(
    int pad_index,
    mojom::GamepadHapticsManager::ResetVibrationActuatorCallback callback) {
  PadState* pad_state = GetConnectedPadState(pad_index);
  if (!pad_state) {
    std::move(callback).Run(
        mojom::GamepadHapticsResult::GamepadHapticsResultError);
    return;
  }

  GamepadDataFetcher* fetcher = GetSourceGamepadDataFetcher(pad_state->source);
  if (!fetcher) {
    std::move(callback).Run(
        mojom::GamepadHapticsResult::GamepadHapticsResultNotSupported);
    return;
  }

  fetcher->ResetVibration(pad_state->source_id, std::move(callback));
}
