void GamepadProvider::PlayVibrationEffectOnce(
    int pad_index,
    mojom::GamepadHapticEffectType type,
    mojom::GamepadEffectParametersPtr params,
    mojom::GamepadHapticsManager::PlayVibrationEffectOnceCallback callback) {
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

  fetcher->PlayEffect(pad_state->source_id, type, std::move(params),
                      std::move(callback));
}
