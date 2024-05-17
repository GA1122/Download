initialize_readline ()
{
  rl_command_func_t *func;
  char kseq[2];

  if (bash_readline_initialized)
    return;

  rl_terminal_name = get_string_value ("TERM");
  rl_instream = stdin;
  rl_outstream = stderr;

   
  rl_readline_name = "Bash";

   
  rl_add_defun ("shell-expand-line", shell_expand_line, -1);
#ifdef BANG_HISTORY
  rl_add_defun ("history-expand-line", history_expand_line, -1);
  rl_add_defun ("magic-space", tcsh_magic_space, -1);
#endif

  rl_add_defun ("shell-forward-word", bash_forward_shellword, -1);
  rl_add_defun ("shell-backward-word", bash_backward_shellword, -1);
  rl_add_defun ("shell-kill-word", bash_kill_shellword, -1);
  rl_add_defun ("shell-backward-kill-word", bash_backward_kill_shellword, -1);

#ifdef ALIAS
  rl_add_defun ("alias-expand-line", alias_expand_line, -1);
#  ifdef BANG_HISTORY
  rl_add_defun ("history-and-alias-expand-line", history_and_alias_expand_line, -1);
#  endif
#endif

   
  rl_add_defun ("insert-last-argument", rl_yank_last_arg, -1);

  rl_add_defun ("operate-and-get-next", operate_and_get_next, -1);
  rl_add_defun ("display-shell-version", display_shell_version, -1);
  rl_add_defun ("edit-and-execute-command", emacs_edit_and_execute_command, -1);

#if defined (BRACE_COMPLETION)
  rl_add_defun ("complete-into-braces", bash_brace_completion, -1);
#endif

#if defined (SPECIFIC_COMPLETION_FUNCTIONS)
  rl_add_defun ("complete-filename", bash_complete_filename, -1);
  rl_add_defun ("possible-filename-completions", bash_possible_filename_completions, -1);
  rl_add_defun ("complete-username", bash_complete_username, -1);
  rl_add_defun ("possible-username-completions", bash_possible_username_completions, -1);
  rl_add_defun ("complete-hostname", bash_complete_hostname, -1);
  rl_add_defun ("possible-hostname-completions", bash_possible_hostname_completions, -1);
  rl_add_defun ("complete-variable", bash_complete_variable, -1);
  rl_add_defun ("possible-variable-completions", bash_possible_variable_completions, -1);
  rl_add_defun ("complete-command", bash_complete_command, -1);
  rl_add_defun ("possible-command-completions", bash_possible_command_completions, -1);
  rl_add_defun ("glob-complete-word", bash_glob_complete_word, -1);
  rl_add_defun ("glob-expand-word", bash_glob_expand_word, -1);
  rl_add_defun ("glob-list-expansions", bash_glob_list_expansions, -1);
#endif

  rl_add_defun ("dynamic-complete-history", dynamic_complete_history, -1);
  rl_add_defun ("dabbrev-expand", bash_dabbrev_expand, -1);

   
  if (RL_ISSTATE(RL_STATE_INITIALIZED) == 0)
    rl_initialize ();

   
  rl_bind_key_if_unbound_in_map (CTRL('E'), shell_expand_line, emacs_meta_keymap);

#ifdef BANG_HISTORY
  rl_bind_key_if_unbound_in_map ('^', history_expand_line, emacs_meta_keymap);
#endif

  rl_bind_key_if_unbound_in_map (CTRL ('O'), operate_and_get_next, emacs_standard_keymap);
  rl_bind_key_if_unbound_in_map (CTRL ('V'), display_shell_version, emacs_ctlx_keymap);

   
  kseq[0] = CTRL('J');
  kseq[1] = '\0';
  func = rl_function_of_keyseq (kseq, emacs_meta_keymap, (int *)NULL);
  if (func == rl_vi_editing_mode)
    rl_unbind_key_in_map (CTRL('J'), emacs_meta_keymap);
  kseq[0] = CTRL('M');
  func = rl_function_of_keyseq (kseq, emacs_meta_keymap, (int *)NULL);
  if (func == rl_vi_editing_mode)
    rl_unbind_key_in_map (CTRL('M'), emacs_meta_keymap);
#if defined (VI_MODE)
  rl_unbind_key_in_map (CTRL('E'), vi_movement_keymap);
#endif

#if defined (BRACE_COMPLETION)
  rl_bind_key_if_unbound_in_map ('{', bash_brace_completion, emacs_meta_keymap);  
#endif  

#if defined (SPECIFIC_COMPLETION_FUNCTIONS)
  rl_bind_key_if_unbound_in_map ('/', bash_complete_filename, emacs_meta_keymap);
  rl_bind_key_if_unbound_in_map ('/', bash_possible_filename_completions, emacs_ctlx_keymap);

   
  kseq[0] = '~';
  kseq[1] = '\0';
  func = rl_function_of_keyseq (kseq, emacs_meta_keymap, (int *)NULL);
  if (func == 0 || func == rl_tilde_expand)
    rl_bind_keyseq_in_map (kseq, bash_complete_username, emacs_meta_keymap);

  rl_bind_key_if_unbound_in_map ('~', bash_possible_username_completions, emacs_ctlx_keymap);

  rl_bind_key_if_unbound_in_map ('@', bash_complete_hostname, emacs_meta_keymap);
  rl_bind_key_if_unbound_in_map ('@', bash_possible_hostname_completions, emacs_ctlx_keymap);

  rl_bind_key_if_unbound_in_map ('$', bash_complete_variable, emacs_meta_keymap);
  rl_bind_key_if_unbound_in_map ('$', bash_possible_variable_completions, emacs_ctlx_keymap);

  rl_bind_key_if_unbound_in_map ('!', bash_complete_command, emacs_meta_keymap);
  rl_bind_key_if_unbound_in_map ('!', bash_possible_command_completions, emacs_ctlx_keymap);

  rl_bind_key_if_unbound_in_map ('g', bash_glob_complete_word, emacs_meta_keymap);
  rl_bind_key_if_unbound_in_map ('*', bash_glob_expand_word, emacs_ctlx_keymap);
  rl_bind_key_if_unbound_in_map ('g', bash_glob_list_expansions, emacs_ctlx_keymap);

#endif  

  kseq[0] = TAB;
  kseq[1] = '\0';
  func = rl_function_of_keyseq (kseq, emacs_meta_keymap, (int *)NULL);
  if (func == 0 || func == rl_tab_insert)
    rl_bind_key_in_map (TAB, dynamic_complete_history, emacs_meta_keymap);

   
  rl_attempted_completion_function = attempt_shell_completion;

   
  set_directory_hook ();

  rl_filename_rewrite_hook = bash_filename_rewrite_hook;

  rl_filename_stat_hook = bash_filename_stat_hook;

   
  rl_ignore_some_completions_function = filename_completion_ignore;

   
  rl_bind_key_if_unbound_in_map (CTRL ('E'), emacs_edit_and_execute_command, emacs_ctlx_keymap);
#if defined (VI_MODE)
  rl_bind_key_if_unbound_in_map ('v', vi_edit_and_execute_command, vi_movement_keymap);
#  if defined (ALIAS)
  rl_bind_key_if_unbound_in_map ('@', posix_edit_macros, vi_movement_keymap);
#  endif

  rl_bind_key_in_map ('\\', bash_vi_complete, vi_movement_keymap);
  rl_bind_key_in_map ('*', bash_vi_complete, vi_movement_keymap);
  rl_bind_key_in_map ('=', bash_vi_complete, vi_movement_keymap);
#endif

  rl_completer_quote_characters = "'\"";

   
  enable_hostname_completion (perform_hostname_completion);

   
  rl_filename_quote_characters = default_filename_quote_characters;
  set_filename_bstab (rl_filename_quote_characters);

  rl_filename_quoting_function = bash_quote_filename;
  rl_filename_dequoting_function = bash_dequote_filename;
  rl_char_is_quoted_p = char_is_quoted;

#if 0
   
  if (posixly_correct)
    posix_readline_initialize (1);
#endif

  bash_readline_initialized = 1;
}
