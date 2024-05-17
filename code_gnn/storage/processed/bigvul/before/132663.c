void BlinkTestRunner::ReportLeakDetectionResult(
    const LeakDetectionResult& report) {
  Send(new ShellViewHostMsg_LeakDetectionDone(routing_id(), report));
}
