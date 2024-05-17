Backtrace::Backtrace(pid_t pid, pid_t tid, BacktraceMap* map)
 : pid_(pid), tid_(tid), map_(map), map_shared_(true) {
 if (map_ == nullptr) {
    map_ = BacktraceMap::Create(pid);
    map_shared_ = false;
 }
}
