VOID CALLBACK tq_timer_cb(PVOID arg, BOOLEAN timed_out)
{
	zend_bool *php_timed_out;

	 
	if (!timed_out) {
		return;
	}

	php_timed_out = (zend_bool *)arg;
	*php_timed_out = 1;
}
