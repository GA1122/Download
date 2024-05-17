void IPCThreadState::flushCommands()
{
 if (mProcess->mDriverFD <= 0)
 return;
    talkWithDriver(false);
}
