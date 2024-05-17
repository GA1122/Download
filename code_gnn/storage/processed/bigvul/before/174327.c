static const void* printReturnCommand(TextOutput& out, const void* _cmd)
{
 static const size_t N = sizeof(kReturnStrings)/sizeof(kReturnStrings[0]);
 const int32_t* cmd = (const int32_t*)_cmd;
 uint32_t code = (uint32_t)*cmd++;
 size_t cmdIndex = code & 0xff;
 if (code == BR_ERROR) {
        out << "BR_ERROR: " << (void*)(long)(*cmd++) << endl;
 return cmd;
 } else if (cmdIndex >= N) {
        out << "Unknown reply: " << code << endl;
 return cmd;
 }
    out << kReturnStrings[cmdIndex];
 
 switch (code) {
 case BR_TRANSACTION:
 case BR_REPLY: {
            out << ": " << indent;
            cmd = (const int32_t *)printBinderTransactionData(out, cmd);
            out << dedent;
 } break;
 
 case BR_ACQUIRE_RESULT: {
 const int32_t res = *cmd++;
            out << ": " << res << (res ? " (SUCCESS)" : " (FAILURE)");
 } break;
 
 case BR_INCREFS:
 case BR_ACQUIRE:
 case BR_RELEASE:
 case BR_DECREFS: {
 const int32_t b = *cmd++;
 const int32_t c = *cmd++;
            out << ": target=" << (void*)(long)b << " (cookie " << (void*)(long)c << ")";
 } break;
 
 case BR_ATTEMPT_ACQUIRE: {
 const int32_t p = *cmd++;
 const int32_t b = *cmd++;
 const int32_t c = *cmd++;
            out << ": target=" << (void*)(long)b << " (cookie " << (void*)(long)c
 << "), pri=" << p;
 } break;

 case BR_DEAD_BINDER:
 case BR_CLEAR_DEATH_NOTIFICATION_DONE: {
 const int32_t c = *cmd++;
            out << ": death cookie " << (void*)(long)c;
 } break;

 default:
 break;
 }
 
    out << endl;
 return cmd;
}
