void RecordAppLauncherPromoHistogram(
      apps::AppLauncherPromoHistogramValues value) {
  DCHECK_LT(value, apps::APP_LAUNCHER_PROMO_MAX);
  UMA_HISTOGRAM_ENUMERATION(
      "Apps.AppLauncherPromo", value, apps::APP_LAUNCHER_PROMO_MAX);
}
