set_saved_history ()
{
   
  if (HISTORY_FULL () &&
      hist_last_line_added == 0 &&
      saved_history_line_to_use < history_length - 1)
    saved_history_line_to_use++;

  if (saved_history_line_to_use >= 0)
    {
     rl_get_previous_history (history_length - saved_history_line_to_use, 0);
     last_saved_history_line = saved_history_line_to_use;
    }
  saved_history_line_to_use = -1;
  rl_startup_hook = old_rl_startup_hook;
  return (0);
}