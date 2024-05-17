void PwnMessageHelper::LockMouse(RenderProcessHost* process,
                                 int routing_id,
                                 bool user_gesture,
                                 bool privileged) {
  IPC::IpcSecurityTestUtil::PwnMessageReceived(
      process->GetChannel(),
      ViewHostMsg_LockMouse(routing_id, user_gesture, privileged));
}
