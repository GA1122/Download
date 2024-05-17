 void RenderThreadImpl::RecordUserMetrics(const std::string& action) {
   Send(new ViewHostMsg_UserMetricsRecordAction(action));
 }
