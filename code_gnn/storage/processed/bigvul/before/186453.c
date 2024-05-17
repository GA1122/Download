 void RenderFrameHostImpl::OnDidAddMessageToConsole(
    int32_t level,
// void RenderFrameHostImpl::DidAddMessageToConsole(
//     blink::mojom::ConsoleMessageLevel log_level,
      const base::string16& message,
      int32_t line_no,
      const base::string16& source_id) {
  if (level < logging::LOG_VERBOSE || level > logging::LOG_FATAL) {
    bad_message::ReceivedBadMessage(
        GetProcess(), bad_message::RFH_DID_ADD_CONSOLE_MESSAGE_BAD_SEVERITY);
    return;
//    
//    
//   logging::LogSeverity log_severity = logging::LOG_VERBOSE;
//   switch (log_level) {
//     case blink::mojom::ConsoleMessageLevel::kVerbose:
//       log_severity = logging::LOG_VERBOSE;
//       break;
//     case blink::mojom::ConsoleMessageLevel::kInfo:
//       log_severity = logging::LOG_INFO;
//       break;
//     case blink::mojom::ConsoleMessageLevel::kWarning:
//       log_severity = logging::LOG_WARNING;
//       break;
//     case blink::mojom::ConsoleMessageLevel::kError:
//       log_severity = logging::LOG_ERROR;
//       break;
    }
  
  if (delegate_->DidAddMessageToConsole(level, message, line_no, source_id))
//   if (delegate_->DidAddMessageToConsole(log_severity, message, line_no,
//                                         source_id)) {
      return;
//   }
  
//    
    const bool is_builtin_component =
        HasWebUIScheme(delegate_->GetMainFrameLastCommittedURL()) ||
       GetContentClient()->browser()->IsBuiltinComponent(
           GetProcess()->GetBrowserContext(), GetLastCommittedOrigin());
    const bool is_off_the_record =
        GetSiteInstance()->GetBrowserContext()->IsOffTheRecord();
  
  LogConsoleMessage(level, message, line_no, is_builtin_component,
//   LogConsoleMessage(log_severity, message, line_no, is_builtin_component,
                      is_off_the_record, source_id);
  }