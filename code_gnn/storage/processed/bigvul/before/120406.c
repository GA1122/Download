GestureProviderAura::GestureProviderAura(GestureProviderAuraClient* client)
    : client_(client),
      filtered_gesture_provider_(ui::DefaultGestureProviderConfig(), this),
      handling_event_(false) {
  filtered_gesture_provider_.SetDoubleTapSupportForPlatformEnabled(false);
}
