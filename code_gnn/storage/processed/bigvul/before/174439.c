static void dump_map(log_t* log, const backtrace_map_t* map, bool fault_addr) {
  _LOG(log, logtype::MAPS, "%s%" PRIPTR "-%" PRIPTR " %c%c%c  %7" PRIdPTR "  %s\n",
 (fault_addr? "--->" : "    "), map->start, map->end - 1,
 (map->flags & PROT_READ) ? 'r' : '-', (map->flags & PROT_WRITE) ? 'w' : '-',
 (map->flags & PROT_EXEC) ? 'x' : '-',
 (map->end - map->start), map->name.c_str());
}
