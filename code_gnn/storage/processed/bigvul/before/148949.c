  __declspec(naked) __inline sqlite_uint64 __cdecl sqlite3Hwtime(void){
     __asm {
        rdtsc
        ret       ; return value at EDX:EAX
     }
  }
