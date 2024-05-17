PHP_GINIT_FUNCTION(phar)  
{
	phar_mime_type mime;

	memset(phar_globals, 0, sizeof(zend_phar_globals));
	phar_globals->readonly = 1;

	zend_hash_init(&phar_globals->mime_types, 0, NULL, NULL, 1);

#define PHAR_SET_MIME(mimetype, ret, fileext) \
		mime.mime = mimetype; \
		mime.len = sizeof((mimetype))+1; \
		mime.type = ret; \
		zend_hash_add(&phar_globals->mime_types, fileext, sizeof(fileext)-1, (void *)&mime, sizeof(phar_mime_type), NULL); \

	PHAR_SET_MIME("text/html", PHAR_MIME_PHPS, "phps")
	PHAR_SET_MIME("text/plain", PHAR_MIME_OTHER, "c")
	PHAR_SET_MIME("text/plain", PHAR_MIME_OTHER, "cc")
	PHAR_SET_MIME("text/plain", PHAR_MIME_OTHER, "cpp")
	PHAR_SET_MIME("text/plain", PHAR_MIME_OTHER, "c++")
	PHAR_SET_MIME("text/plain", PHAR_MIME_OTHER, "dtd")
	PHAR_SET_MIME("text/plain", PHAR_MIME_OTHER, "h")
	PHAR_SET_MIME("text/plain", PHAR_MIME_OTHER, "log")
	PHAR_SET_MIME("text/plain", PHAR_MIME_OTHER, "rng")
	PHAR_SET_MIME("text/plain", PHAR_MIME_OTHER, "txt")
	PHAR_SET_MIME("text/plain", PHAR_MIME_OTHER, "xsd")
	PHAR_SET_MIME("", PHAR_MIME_PHP, "php")
	PHAR_SET_MIME("", PHAR_MIME_PHP, "inc")
	PHAR_SET_MIME("video/avi", PHAR_MIME_OTHER, "avi")
	PHAR_SET_MIME("image/bmp", PHAR_MIME_OTHER, "bmp")
	PHAR_SET_MIME("text/css", PHAR_MIME_OTHER, "css")
	PHAR_SET_MIME("image/gif", PHAR_MIME_OTHER, "gif")
	PHAR_SET_MIME("text/html", PHAR_MIME_OTHER, "htm")
	PHAR_SET_MIME("text/html", PHAR_MIME_OTHER, "html")
	PHAR_SET_MIME("text/html", PHAR_MIME_OTHER, "htmls")
	PHAR_SET_MIME("image/x-ico", PHAR_MIME_OTHER, "ico")
	PHAR_SET_MIME("image/jpeg", PHAR_MIME_OTHER, "jpe")
	PHAR_SET_MIME("image/jpeg", PHAR_MIME_OTHER, "jpg")
	PHAR_SET_MIME("image/jpeg", PHAR_MIME_OTHER, "jpeg")
	PHAR_SET_MIME("application/x-javascript", PHAR_MIME_OTHER, "js")
	PHAR_SET_MIME("audio/midi", PHAR_MIME_OTHER, "midi")
	PHAR_SET_MIME("audio/midi", PHAR_MIME_OTHER, "mid")
	PHAR_SET_MIME("audio/mod", PHAR_MIME_OTHER, "mod")
	PHAR_SET_MIME("movie/quicktime", PHAR_MIME_OTHER, "mov")
	PHAR_SET_MIME("audio/mp3", PHAR_MIME_OTHER, "mp3")
	PHAR_SET_MIME("video/mpeg", PHAR_MIME_OTHER, "mpg")
	PHAR_SET_MIME("video/mpeg", PHAR_MIME_OTHER, "mpeg")
	PHAR_SET_MIME("application/pdf", PHAR_MIME_OTHER, "pdf")
	PHAR_SET_MIME("image/png", PHAR_MIME_OTHER, "png")
	PHAR_SET_MIME("application/shockwave-flash", PHAR_MIME_OTHER, "swf")
	PHAR_SET_MIME("image/tiff", PHAR_MIME_OTHER, "tif")
	PHAR_SET_MIME("image/tiff", PHAR_MIME_OTHER, "tiff")
	PHAR_SET_MIME("audio/wav", PHAR_MIME_OTHER, "wav")
	PHAR_SET_MIME("image/xbm", PHAR_MIME_OTHER, "xbm")
	PHAR_SET_MIME("text/xml", PHAR_MIME_OTHER, "xml")

	phar_restore_orig_functions(TSRMLS_C);
}
 