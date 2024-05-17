static void prplcb_xfer_progress(PurpleXfer *xfer, double percent)
{
	struct prpl_xfer_data *px = xfer->ui_data;

	if (px == NULL) {
		return;
	}

	if (purple_xfer_get_type(xfer) == PURPLE_XFER_SEND) {
		if (*px->fn) {
			char *slash;

			unlink(px->fn);
			if ((slash = strrchr(px->fn, '/'))) {
				*slash = '\0';
				rmdir(px->fn);
			}
			*px->fn = '\0';
		}

		return;
	}

	if (px->fd == -1 && percent > 0) {
		 
		px->fd = open(px->fn, O_RDONLY);

		 
		unlink(px->fn);
	}

	if (percent < 1) {
		try_write_to_ui(px->ft, 0, 0);
	} else {
		 
		b_timeout_add(0, try_write_to_ui, px->ft);
	}
}
