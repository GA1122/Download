void RenderFrameImpl::didHandleOnloadEvents(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  if (!frame->parent()) {
    FrameMsg_UILoadMetricsReportType::Value report_type =
        static_cast<FrameMsg_UILoadMetricsReportType::Value>(
            frame->dataSource()->request().inputPerfMetricReportPolicy());
    base::TimeTicks ui_timestamp = base::TimeTicks() +
        base::TimeDelta::FromSecondsD(
            frame->dataSource()->request().uiStartTime());

    Send(new FrameHostMsg_DocumentOnLoadCompleted(
        routing_id_, report_type, ui_timestamp));
  }
}
