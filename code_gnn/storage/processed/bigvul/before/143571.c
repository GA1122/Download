void OomInterventionTabHelper::OnNearOomDetected() {
  DCHECK(!OomInterventionConfig::GetInstance()->should_detect_in_renderer());
  DCHECK_EQ(web_contents()->GetVisibility(), content::Visibility::VISIBLE);
  DCHECK(!near_oom_detected_time_);
  subscription_.reset();

  StartDetectionInRenderer();
  DCHECK(!renderer_detection_timer_.IsRunning());
  renderer_detection_timer_.Start(
      FROM_HERE, kRendererHighMemoryUsageDetectionWindow,
      base::BindRepeating(&OomInterventionTabHelper::
                              OnDetectionWindowElapsedWithoutHighMemoryUsage,
                          weak_ptr_factory_.GetWeakPtr()));
}
