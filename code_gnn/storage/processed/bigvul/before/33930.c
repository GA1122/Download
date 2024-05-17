pick_ipc_type(enum qb_ipc_type requested)
{
    const char *env = getenv("PCMK_ipc_type");

    if(env && strcmp("shared-mem", env) == 0) {
        return QB_IPC_SHM;
    } else if(env && strcmp("socket", env) == 0) {
        return QB_IPC_SOCKET;
    } else if(env && strcmp("posix", env) == 0) {
        return QB_IPC_POSIX_MQ;
    } else if(env && strcmp("sysv", env) == 0) {
        return QB_IPC_SYSV_MQ;
    } else if(requested == QB_IPC_NATIVE) {
         
        return QB_IPC_SOCKET;
    }
    return requested;
}