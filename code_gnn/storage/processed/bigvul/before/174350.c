std::string Backtrace::FormatFrameData(const backtrace_frame_data_t* frame) {
 const char* map_name;
 if (BacktraceMap::IsValid(frame->map) && !frame->map.name.empty()) {
    map_name = frame->map.name.c_str();
 } else {
    map_name = "<unknown>";
 }

 uintptr_t relative_pc = BacktraceMap::GetRelativePc(frame->map, frame->pc);

  std::string line(StringPrintf("#%02zu pc %" PRIPTR "  %s", frame->num, relative_pc, map_name));
 if (frame->map.offset != 0) {
    line += " (offset " + StringPrintf("0x%" PRIxPTR, frame->map.offset) + ")";
 }
 if (!frame->func_name.empty()) {
    line += " (" + frame->func_name;
 if (frame->func_offset) {
      line += StringPrintf("+%" PRIuPTR, frame->func_offset);
 }
    line += ')';
 }

 return line;
}
