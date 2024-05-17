static long php_jpeg_emit_message(j_common_ptr jpeg_info, int level)
{
	char message[JMSG_LENGTH_MAX];
	jmpbuf_wrapper *jmpbufw;
	int ignore_warning = 0;

	jmpbufw = (jmpbuf_wrapper *) jpeg_info->client_data;

	if (jmpbufw != 0) {
		ignore_warning = jmpbufw->ignore_warning;
	}

	(jpeg_info->err->format_message)(jpeg_info,message);

	 
	if (level < 0) {
		 
		if ((jpeg_info->err->num_warnings == 0) || (jpeg_info->err->trace_level >= 3)) {
			if (!ignore_warning) {
				gd_error("gd-jpeg, libjpeg: recoverable error: %s\n", message);
			}
		}

		jpeg_info->err->num_warnings++;
	} else {
		 
		if (jpeg_info->err->trace_level >= level) {
			if (!ignore_warning) {
				gd_error("gd-jpeg, libjpeg: strace message: %s\n", message);
			}
		}
	}
	return 1;
}
