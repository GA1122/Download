static int xt_get_next_timeout(GSource *source)
{
  static int has_compatible_appcontext = -1;
  if (has_compatible_appcontext < 0) {
	if ((has_compatible_appcontext = xt_has_compatible_appcontext()) == 0)
	  npw_printf("WARNING: xt_get_next_timeout() is not optimizable\n");
  }
  int timeout = XT_DEFAULT_TIMEOUT;
  if (has_compatible_appcontext) {
	int input_timeout, timer_timeout;
	 
	if (get_appcontext_input_count() > 0)
	  input_timeout = XT_DEFAULT_TIMEOUT;
	else
	  input_timeout = -1;
	 
	if (x_app_context->timerQueue == NULL)
	  timer_timeout = -1;
	else {
	   
	  struct timeval *next = &x_app_context->timerQueue->te_timer_value;
	  GTimeVal now;
	  int64_t diff;
	  g_source_get_current_time(source, &now);
	  if ((diff = (int64_t)next->tv_sec - (int64_t)now.tv_sec) < 0)
		timer_timeout = 0;
	  else if ((diff = diff*1000 + ((int64_t)next->tv_usec - (int64_t)now.tv_usec)/1000) <= 0)
		timer_timeout = 0;
	  else
		timer_timeout = diff;
	}
	if (input_timeout < 0)
	  timeout = timer_timeout;
	else if (timer_timeout < 0)
	  timeout = input_timeout;
	else
	  timeout = MIN(input_timeout, timer_timeout);
  }
  return timeout;
}