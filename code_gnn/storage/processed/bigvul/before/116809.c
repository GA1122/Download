void WebRTCAudioDeviceTest::WaitForMessageLoopCompletion(
    base::MessageLoopProxy* loop) {
  base::WaitableEvent* event = new base::WaitableEvent(false, false);
  loop->PostTask(FROM_HERE, base::Bind(&base::WaitableEvent::Signal,
                 base::Unretained(event)));
  if (event->TimedWait(base::TimeDelta::FromMilliseconds(
          TestTimeouts::action_max_timeout_ms()))) {
    delete event;
  } else {
    ADD_FAILURE() << "Failed to wait for message loop";
  }
}
