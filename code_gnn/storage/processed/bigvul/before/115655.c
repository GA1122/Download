ACTION_P(QuitMainMessageLoop, message_loop) {
  PostQuitTask(message_loop);
}
