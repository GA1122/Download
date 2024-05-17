initialize_subshell ()
{
#if defined (ALIAS)
   
  delete_all_aliases ();
#endif  

#if defined (HISTORY)
   
  history_lines_this_session = 0;
#endif

#if defined (JOB_CONTROL)
   
  without_job_control ();
  set_sigchld_handler ();
  init_job_stats ();
#endif  

   
  reset_shell_flags ();
  reset_shell_options ();
  reset_shopt_options ();

   
  if (vc_isbltnenv (shell_variables))
    shell_variables = shell_variables->down;

  clear_unwind_protect_list (0);
   
  parse_and_execute_level = 0;		 

   
  variable_context = return_catch_flag = funcnest = 0;

  executing_list = 0;		 

   
  if (interactive_shell == 0)
    unset_bash_input (0);
}
