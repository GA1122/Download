bool BrowserMainParts::MainMessageLoopRun(int* result_code) {
  MessagePump::Get()->Start();
  return true;
}
