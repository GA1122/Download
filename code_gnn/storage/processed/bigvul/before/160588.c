void RenderFrameImpl::DidHandleOnloadEvents() {
  if (!frame_->Parent()) {
    FrameMsg_UILoadMetricsReportType::Value report_type =
        static_cast<FrameMsg_UILoadMetricsReportType::Value>(
            frame_->GetDocumentLoader()
                ->GetRequest()
                .InputPerfMetricReportPolicy());
    base::TimeTicks ui_timestamp =
        base::TimeTicks() +
        base::TimeDelta::FromSecondsD(
            frame_->GetDocumentLoader()->GetRequest().UiStartTime());

    Send(new FrameHostMsg_DocumentOnLoadCompleted(
        routing_id_, report_type, ui_timestamp));
  }
}
