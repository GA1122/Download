InputDispatcher::CommandEntry* InputDispatcher::postCommandLocked(Command command) {
 CommandEntry* commandEntry = new CommandEntry(command);
    mCommandQueue.enqueueAtTail(commandEntry);
 return commandEntry;
}
