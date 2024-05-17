IPCThreadState* IPCThreadState::selfOrNull()
{
 if (gHaveTLS) {
 const pthread_key_t k = gTLS;
 IPCThreadState* st = (IPCThreadState*)pthread_getspecific(k);
 return st;
 }
 return NULL;
}
