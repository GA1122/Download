static uint64_t getUniqueId() {
 static volatile int32_t nextId = 0;
 uint64_t id = static_cast<uint64_t>(getpid()) << 32;
    id |= static_cast<uint32_t>(android_atomic_inc(&nextId));
 return id;
}
