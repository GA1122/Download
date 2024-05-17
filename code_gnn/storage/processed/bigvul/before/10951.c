static void exif_process_TIFF_in_JPEG(image_info_type *ImageInfo, char *CharBuf, size_t length, size_t displacement TSRMLS_DC)
{
	unsigned exif_value_2a, offset_of_ifd;

	 
	if (memcmp(CharBuf, "II", 2) == 0) {
		ImageInfo->motorola_intel = 0;
	} else if (memcmp(CharBuf, "MM", 2) == 0) {
		ImageInfo->motorola_intel = 1;
	} else {
		exif_error_docref(NULL EXIFERR_CC, ImageInfo, E_WARNING, "Invalid TIFF alignment marker");
		return;
	}

	 
	exif_value_2a = php_ifd_get16u(CharBuf+2, ImageInfo->motorola_intel);
	offset_of_ifd = php_ifd_get32u(CharBuf+4, ImageInfo->motorola_intel);
	if ( exif_value_2a != 0x2a || offset_of_ifd < 0x08) {
		exif_error_docref(NULL EXIFERR_CC, ImageInfo, E_WARNING, "Invalid TIFF start (1)");
		return;
	}
        }
