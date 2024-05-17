static const char* get_sigcode(int signo, int code) {
 switch (signo) {
 case SIGILL:
 switch (code) {
 case ILL_ILLOPC: return "ILL_ILLOPC";
 case ILL_ILLOPN: return "ILL_ILLOPN";
 case ILL_ILLADR: return "ILL_ILLADR";
 case ILL_ILLTRP: return "ILL_ILLTRP";
 case ILL_PRVOPC: return "ILL_PRVOPC";
 case ILL_PRVREG: return "ILL_PRVREG";
 case ILL_COPROC: return "ILL_COPROC";
 case ILL_BADSTK: return "ILL_BADSTK";
 }
 static_assert(NSIGILL == ILL_BADSTK, "missing ILL_* si_code");
 break;
 case SIGBUS:
 switch (code) {
 case BUS_ADRALN: return "BUS_ADRALN";
 case BUS_ADRERR: return "BUS_ADRERR";
 case BUS_OBJERR: return "BUS_OBJERR";
 case BUS_MCEERR_AR: return "BUS_MCEERR_AR";
 case BUS_MCEERR_AO: return "BUS_MCEERR_AO";
 }
 static_assert(NSIGBUS == BUS_MCEERR_AO, "missing BUS_* si_code");
 break;
 case SIGFPE:
 switch (code) {
 case FPE_INTDIV: return "FPE_INTDIV";
 case FPE_INTOVF: return "FPE_INTOVF";
 case FPE_FLTDIV: return "FPE_FLTDIV";
 case FPE_FLTOVF: return "FPE_FLTOVF";
 case FPE_FLTUND: return "FPE_FLTUND";
 case FPE_FLTRES: return "FPE_FLTRES";
 case FPE_FLTINV: return "FPE_FLTINV";
 case FPE_FLTSUB: return "FPE_FLTSUB";
 }
 static_assert(NSIGFPE == FPE_FLTSUB, "missing FPE_* si_code");
 break;
 case SIGSEGV:
 switch (code) {
 case SEGV_MAPERR: return "SEGV_MAPERR";
 case SEGV_ACCERR: return "SEGV_ACCERR";
 }
 static_assert(NSIGSEGV == SEGV_ACCERR, "missing SEGV_* si_code");
 break;
 case SIGTRAP:
 switch (code) {
 case TRAP_BRKPT: return "TRAP_BRKPT";
 case TRAP_TRACE: return "TRAP_TRACE";
 case TRAP_BRANCH: return "TRAP_BRANCH";
 case TRAP_HWBKPT: return "TRAP_HWBKPT";
 }
 static_assert(NSIGTRAP == TRAP_HWBKPT, "missing TRAP_* si_code");
 break;
 }
 switch (code) {
 case SI_USER: return "SI_USER";
 case SI_KERNEL: return "SI_KERNEL";
 case SI_QUEUE: return "SI_QUEUE";
 case SI_TIMER: return "SI_TIMER";
 case SI_MESGQ: return "SI_MESGQ";
 case SI_ASYNCIO: return "SI_ASYNCIO";
 case SI_SIGIO: return "SI_SIGIO";
 case SI_TKILL: return "SI_TKILL";
 case SI_DETHREAD: return "SI_DETHREAD";
 }
 return "?";
}
