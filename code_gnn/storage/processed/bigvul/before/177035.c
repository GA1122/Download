InputDispatcher::EventEntry::~EventEntry() {
    releaseInjectionState();
}