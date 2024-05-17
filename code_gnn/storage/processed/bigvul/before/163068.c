static bool CreateInitProcessReaper(base::Closure* post_fork_parent_callback) {
  const bool init_created =
      sandbox::CreateInitProcessReaper(post_fork_parent_callback);
  if (!init_created) {
    LOG(ERROR) << "Error creating an init process to reap zombies";
    return false;
  }
  return true;
}
