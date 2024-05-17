  void AppStatusChanged(base::android::ApplicationState state) {
    if (variations::GetVariationParamByFeatureAsBool(
            kContentSuggestionsNotificationsFeature,
            kContentSuggestionsNotificationsKeepNotificationWhenFrontmostParam,
            false)) {
      return;
    }
    if (!ShouldNotifyInState(state)) {
      ContentSuggestionsNotificationHelper::HideAllNotifications(
          CONTENT_SUGGESTIONS_HIDE_FRONTMOST);
    }
  }
