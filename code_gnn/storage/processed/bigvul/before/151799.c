void RecordOffliningPreviewsUMA(const ClientId& client_id,
                                content::PreviewsState previews_state) {
  bool is_previews_enabled =
      (previews_state != content::PreviewsTypes::PREVIEWS_OFF &&
       previews_state != content::PreviewsTypes::PREVIEWS_NO_TRANSFORM);

  base::UmaHistogramBoolean(
      AddHistogramSuffix(client_id,
                         "OfflinePages.Background.OffliningPreviewStatus"),
       is_previews_enabled);
 }
