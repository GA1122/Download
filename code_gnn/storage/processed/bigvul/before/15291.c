static void user_space_stream_notifier_dtor(php_stream_notifier *notifier)
{
	if (notifier && notifier->ptr) {
		zval_ptr_dtor((zval **)&(notifier->ptr));
		notifier->ptr = NULL;
	}
}
