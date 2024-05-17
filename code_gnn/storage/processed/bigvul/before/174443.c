static void dump_stack(Backtrace* backtrace, log_t* log) {
 size_t first = 0, last;
 for (size_t i = 0; i < backtrace->NumFrames(); i++) {
 const backtrace_frame_data_t* frame = backtrace->GetFrame(i);
 if (frame->sp) {
 if (!first) {
        first = i+1;
 }
      last = i;
 }
 }
 if (!first) {
 return;
 }
  first--;

 word_t sp = backtrace->GetFrame(first)->sp - STACK_WORDS * sizeof(word_t);
  dump_stack_segment(backtrace, log, &sp, STACK_WORDS, -1);

 for (size_t i = first; i <= last; i++) {
 const backtrace_frame_data_t* frame = backtrace->GetFrame(i);
 if (sp != frame->sp) {
      _LOG(log, logtype::STACK, "         ........  ........\n");
      sp = frame->sp;
 }
 if (i == last) {
      dump_stack_segment(backtrace, log, &sp, STACK_WORDS, i);
 if (sp < frame->sp + frame->stack_size) {
        _LOG(log, logtype::STACK, "         ........  ........\n");
 }
 } else {
 size_t words = frame->stack_size / sizeof(word_t);
 if (words == 0) {
        words = 1;
 } else if (words > STACK_WORDS) {
        words = STACK_WORDS;
 }
      dump_stack_segment(backtrace, log, &sp, words, i);
 }
 }
}
