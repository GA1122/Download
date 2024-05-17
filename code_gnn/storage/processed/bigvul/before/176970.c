uint32_t InputDispatcher::DispatchEntry::nextSeq() {
 uint32_t seq;
 do {
        seq = android_atomic_inc(&sNextSeqAtomic);
 } while (!seq);
 return seq;
}
