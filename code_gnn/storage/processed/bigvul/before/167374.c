void RunClosureAfterCookiesCleared(const base::Closure& task,
                                   uint32_t cookies_deleted) {
  task.Run();
}
