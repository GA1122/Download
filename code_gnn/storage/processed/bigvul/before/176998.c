bool InputDispatcher::runCommandsLockedInterruptible() {
 if (mCommandQueue.isEmpty()) {
 return false;
 }

 do {
 CommandEntry* commandEntry = mCommandQueue.dequeueAtHead();

 Command command = commandEntry->command;
 (this->*command)(commandEntry);  

        commandEntry->connection.clear();
 delete commandEntry;
 } while (! mCommandQueue.isEmpty());
 return true;
}
