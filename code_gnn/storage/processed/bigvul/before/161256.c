Response EmulationHandler::SetEmitTouchEventsForMouse(
    bool enabled,
    Maybe<std::string> configuration) {
  touch_emulation_enabled_ = enabled;
  touch_emulation_configuration_ = configuration.fromMaybe("");
  UpdateTouchEventEmulationState();
  return Response::OK();
}
