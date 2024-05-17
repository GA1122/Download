static const Mem *columnNullValue(void){
   
  static const Mem nullMem 
#if defined(SQLITE_DEBUG) && defined(__GNUC__)
    __attribute__((aligned(8))) 
#endif
    = {
          {0},
          (u16)MEM_Null,
          (u8)0,
          (u8)0,
          (int)0,
          (char*)0,
          (char*)0,
          (int)0,
          (u32)0,
          (sqlite3*)0,
          (void(*)(void*))0,
#ifdef SQLITE_DEBUG
          (Mem*)0,
          (void*)0,
#endif
      };
  return &nullMem;
}
