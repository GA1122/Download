InputDispatcher::CommandEntry::CommandEntry(Command command) :
    command(command), eventTime(0), keyEntry(NULL), userActivityEventType(0),
    seq(0), handled(false) {
}
