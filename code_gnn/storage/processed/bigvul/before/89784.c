propagate_exit_status (int status)
{
  if (WIFEXITED (status))
    return WEXITSTATUS (status);

   
  if (WIFSIGNALED (status))
    return 128 + WTERMSIG (status);

   
  return 255;
}
