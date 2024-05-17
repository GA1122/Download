  void SetManualFallbacksForFillingStandalone(bool enabled) {
    if (enabled) {
      scoped_feature_list_.InitAndEnableFeature(
          password_manager::features::kEnableManualFallbacksFillingStandalone);
    } else {
      scoped_feature_list_.InitAndDisableFeature(
          password_manager::features::kEnableManualFallbacksFillingStandalone);
    }
  }
