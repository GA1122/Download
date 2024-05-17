terminate_workers(pid_t *pids, int bound)
{
    int i, status, num_active = 0;
    pid_t pid;

     
    for (i = 0; i < bound; i++) {
        if (pids[i] == -1)
            continue;
        kill(pids[i], SIGTERM);
        num_active++;
    }

     
    while (num_active > 0) {
        pid = wait(&status);
        if (pid >= 0)
            num_active--;
    }
}
