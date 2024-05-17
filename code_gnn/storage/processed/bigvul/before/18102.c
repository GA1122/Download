_sigusr1(int x __UNUSED__, siginfo_t *info __UNUSED__, void *data __UNUSED__)
{
   struct sigaction action;

    
   stop_ptrace = EINA_TRUE;

   action.sa_sigaction = _sigusr1;
   action.sa_flags = SA_RESETHAND;
   sigemptyset(&action.sa_mask);
   sigaction(SIGUSR1, &action, NULL);
}
