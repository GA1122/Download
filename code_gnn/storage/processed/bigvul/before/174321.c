status_t IPCThreadState::handlePolledCommands()
{
 status_t result;

 do {
        result = getAndExecuteCommand();
 } while (mIn.dataPosition() < mIn.dataSize());

    processPendingDerefs();
    flushCommands();
 return result;
}
