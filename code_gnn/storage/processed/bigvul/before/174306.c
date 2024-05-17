int64_t IPCThreadState::clearCallingIdentity()
{
 int64_t token = ((int64_t)mCallingUid<<32) | mCallingPid;
    clearCaller();
 return token;
}
