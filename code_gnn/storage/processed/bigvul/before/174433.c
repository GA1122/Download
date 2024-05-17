static void dump_abort_message(Backtrace* backtrace, log_t* log, uintptr_t address) {
 if (address == 0) {
 return;
 }

  address += sizeof(size_t);  

 char msg[512];
  memset(msg, 0, sizeof(msg));
 char* p = &msg[0];
 while (p < &msg[sizeof(msg)]) {
 word_t data;
 size_t len = sizeof(word_t);
 if (!backtrace->ReadWord(address, &data)) {
 break;
 }
    address += sizeof(word_t);

 while (len > 0 && (*p++ = (data >> (sizeof(word_t) - len) * 8) & 0xff) != 0)
       len--;
 }
  msg[sizeof(msg) - 1] = '\0';

  _LOG(log, logtype::HEADER, "Abort message: '%s'\n", msg);
}
