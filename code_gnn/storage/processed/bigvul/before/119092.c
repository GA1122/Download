void OutputFrameId(intptr_t frame_id, BacktraceOutputHandler* handler) {
  char buf[30] = { '\0' };
  handler->HandleOutput("#");
  internal::itoa_r(frame_id, buf, sizeof(buf), 10, 1);
  handler->HandleOutput(buf);
}
