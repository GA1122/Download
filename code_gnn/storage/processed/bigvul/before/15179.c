PHP_MINFO_FUNCTION(gd)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "GD Support", "enabled");

	 

	php_info_print_table_row(2, "GD Version", PHP_GD_VERSION_STRING);

#ifdef ENABLE_GD_TTF
	php_info_print_table_row(2, "FreeType Support", "enabled");
#if HAVE_LIBFREETYPE
	php_info_print_table_row(2, "FreeType Linkage", "with freetype");
	{
		char tmp[256];

#ifdef FREETYPE_PATCH
		snprintf(tmp, sizeof(tmp), "%d.%d.%d", FREETYPE_MAJOR, FREETYPE_MINOR, FREETYPE_PATCH);
#elif defined(FREETYPE_MAJOR)
		snprintf(tmp, sizeof(tmp), "%d.%d", FREETYPE_MAJOR, FREETYPE_MINOR);
#else
		snprintf(tmp, sizeof(tmp), "1.x");
#endif
		php_info_print_table_row(2, "FreeType Version", tmp);
	}
#else
	php_info_print_table_row(2, "FreeType Linkage", "with unknown library");
#endif
#endif

#ifdef HAVE_LIBT1
	php_info_print_table_row(2, "T1Lib Support", "enabled");
#endif

 

#ifdef HAVE_GD_GIF_READ
	php_info_print_table_row(2, "GIF Read Support", "enabled");
#endif
#ifdef HAVE_GD_GIF_CREATE
	php_info_print_table_row(2, "GIF Create Support", "enabled");
#endif
#ifdef HAVE_GD_JPG
	{
		char tmp[12];
		snprintf(tmp, sizeof(tmp), "%s", gdJpegGetVersionString());
		php_info_print_table_row(2, "JPEG Support", "enabled");
		php_info_print_table_row(2, "libJPEG Version", tmp);
	}
#endif

#ifdef HAVE_GD_PNG
	php_info_print_table_row(2, "PNG Support", "enabled");
	php_info_print_table_row(2, "libPNG Version", gdPngGetVersionString());
#endif
#ifdef HAVE_GD_WBMP
	php_info_print_table_row(2, "WBMP Support", "enabled");
#endif
#if defined(HAVE_GD_XPM) && defined(HAVE_GD_BUNDLED)
	php_info_print_table_row(2, "XPM Support", "enabled");
	{
		char tmp[12];
		snprintf(tmp, sizeof(tmp), "%d", XpmLibraryVersion());
		php_info_print_table_row(2, "libXpm Version", tmp);
	}
#endif
#ifdef HAVE_GD_XBM
	php_info_print_table_row(2, "XBM Support", "enabled");
#endif
#if defined(USE_GD_JISX0208) && defined(HAVE_GD_BUNDLED)
	php_info_print_table_row(2, "JIS-mapped Japanese Font Support", "enabled");
#endif
	php_info_print_table_end();
	DISPLAY_INI_ENTRIES();
}
