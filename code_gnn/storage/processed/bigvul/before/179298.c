 static void call_console_drivers(unsigned start, unsigned end)
 {
 	unsigned cur_index, start_print;
 	static int msg_level = -1;
 
 	BUG_ON(((int)(start - end)) > 0);
 
 	cur_index = start;
  	start_print = start;
  	while (cur_index != end) {
  		if (msg_level < 0 && ((end - cur_index) > 2)) {
// 			 
// 			char buf_prefix[SYSLOG_PRI_MAX_LENGTH+1];
// 			unsigned i;
// 			for (i = 0; i < ((end - cur_index)) && (i < SYSLOG_PRI_MAX_LENGTH); i++) {
// 				buf_prefix[i] = LOG_BUF(cur_index + i);
// 			}
// 			buf_prefix[i] = '\0';  
// 
  			 
			cur_index += log_prefix(&LOG_BUF(cur_index), &msg_level, NULL);
// 			cur_index += log_prefix((const char *)&buf_prefix, &msg_level, NULL);
  			start_print = cur_index;
  		}
  		while (cur_index != end) {
 			char c = LOG_BUF(cur_index);
 
 			cur_index++;
 			if (c == '\n') {
 				if (msg_level < 0) {
 					 
 					msg_level = default_message_loglevel;
 				}
 				_call_console_drivers(start_print, cur_index, msg_level);
 				msg_level = -1;
 				start_print = cur_index;
 				break;
 			}
 		}
 	}
 	_call_console_drivers(start_print, end, msg_level);
 }