static const char* get_signame(int sig) {
 switch(sig) {
 case SIGABRT: return "SIGABRT";
 case SIGBUS: return "SIGBUS";
 case SIGFPE: return "SIGFPE";
 case SIGILL: return "SIGILL";
 case SIGPIPE: return "SIGPIPE";
 case SIGSEGV: return "SIGSEGV";
#if defined(SIGSTKFLT)
 case SIGSTKFLT: return "SIGSTKFLT";
#endif
 case SIGSTOP: return "SIGSTOP";
 case SIGTRAP: return "SIGTRAP";
 default: return "?";
 }
}
