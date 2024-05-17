InputDispatcher::DispatchEntry::~DispatchEntry() {
    eventEntry->release();
}
