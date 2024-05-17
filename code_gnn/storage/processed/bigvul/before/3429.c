hook_process_child (struct t_hook *hook_process)
{
    char *exec_args[4] = { "sh", "-c", NULL, NULL };
    
     
    close (STDIN_FILENO);
    
     
    close (HOOK_PROCESS(hook_process, child_read[HOOK_PROCESS_STDOUT]));
    close (HOOK_PROCESS(hook_process, child_read[HOOK_PROCESS_STDERR]));
    if (dup2 (HOOK_PROCESS(hook_process, child_write[HOOK_PROCESS_STDOUT]),
              STDOUT_FILENO) < 0)
    {
        _exit (EXIT_FAILURE);
    }
    if (dup2 (HOOK_PROCESS(hook_process, child_write[HOOK_PROCESS_STDERR]),
              STDERR_FILENO) < 0)
    {
        _exit (EXIT_FAILURE);
    }
    
     
    exec_args[2] = HOOK_PROCESS(hook_process, command);
    execvp (exec_args[0], exec_args);
    
     
    fprintf (stderr, "Error with command '%s'\n",
             HOOK_PROCESS(hook_process, command));
    _exit (EXIT_FAILURE);
}
