InputDispatcher::DispatchEntry::DispatchEntry(EventEntry* eventEntry,
 int32_t targetFlags, float xOffset, float yOffset, float scaleFactor) :
        seq(nextSeq()),
        eventEntry(eventEntry), targetFlags(targetFlags),
        xOffset(xOffset), yOffset(yOffset), scaleFactor(scaleFactor),
        deliveryTime(0), resolvedAction(0), resolvedFlags(0) {
    eventEntry->refCount += 1;
}
