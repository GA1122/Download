void RenderViewTest::ProcessPendingMessages() {
  msg_loop_.task_runner()->PostTask(FROM_HERE,
                                    base::MessageLoop::QuitClosure());
  msg_loop_.Run();
}
