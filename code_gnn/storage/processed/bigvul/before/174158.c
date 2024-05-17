bool OMX::livesLocally(node_id  , pid_t pid) {
 return pid == getpid();
}
