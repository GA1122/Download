 static int exif_process_IFD_TAG(image_info_type *ImageInfo, char *dir_entry, char *offset_base, size_t IFDlength, size_t displacement, int section_index, int ReadNextIFD, tag_table_type tag_table TSRMLS_DC)
 {
 	size_t length;
 	int tag, format, components;
 	char *value_ptr, tagname[64], cbuf[32], *outside=NULL;
 	size_t byte_count, offset_val, fpos, fgot;
 	int64_t byte_count_signed;
 	xp_field_type *tmp_xp;
 #ifdef EXIF_DEBUG
 	char *dump_data;
 	int dump_free;
 #endif  
 
 	 
 	if (ImageInfo->ifd_nesting_level > MAX_IFD_NESTING_LEVEL) {
 		exif_error_docref("exif_read_data#error_ifd" EXIFERR_CC, ImageInfo, E_WARNING, "corrupt EXIF header: maximum directory nesting level reached");
 		return FALSE;
 	}
 	ImageInfo->ifd_nesting_level++;
 
 	tag = php_ifd_get16u(dir_entry, ImageInfo->motorola_intel);
 	format = php_ifd_get16u(dir_entry+2, ImageInfo->motorola_intel);
 	components = php_ifd_get32u(dir_entry+4, ImageInfo->motorola_intel);
 
 	if (!format || format > NUM_FORMATS) {
 		 
 		exif_error_docref("exif_read_data#error_ifd" EXIFERR_CC, ImageInfo, E_WARNING, "Process tag(x%04X=%s): Illegal format code 0x%04X, suppose BYTE", tag, exif_get_tagname(tag, tagname, -12, tag_table TSRMLS_CC), format);
 		format = TAG_FMT_BYTE;
 		 
 	}
 
 	if (components < 0) {
 		exif_error_docref("exif_read_data#error_ifd" EXIFERR_CC, ImageInfo, E_WARNING, "Process tag(x%04X=%s): Illegal components(%ld)", tag, exif_get_tagname(tag, tagname, -12, tag_table TSRMLS_CC), components);
 		return FALSE;
 	}
 
 	byte_count_signed = (int64_t)components * php_tiff_bytes_per_format[format];
 
 	if (byte_count_signed < 0 || (byte_count_signed > INT32_MAX)) {
 		exif_error_docref("exif_read_data#error_ifd" EXIFERR_CC, ImageInfo, E_WARNING, "Process tag(x%04X=%s): Illegal byte_count", tag, exif_get_tagname(tag, tagname, -12, tag_table TSRMLS_CC));
 		return FALSE;
 	}
 
 	byte_count = (size_t)byte_count_signed;
 
 	if (byte_count > 4) {
 		offset_val = php_ifd_get32u(dir_entry+8, ImageInfo->motorola_intel);
 		 
 		value_ptr = offset_base+offset_val;
          
 		if (byte_count > IFDlength || offset_val > IFDlength-byte_count || value_ptr < dir_entry || offset_val < (size_t)(dir_entry-offset_base)) {
 			 
 			if (byte_count > ImageInfo->FileSize || offset_val>ImageInfo->FileSize-byte_count || (ImageInfo->FileType!=IMAGE_FILETYPE_TIFF_II && ImageInfo->FileType!=IMAGE_FILETYPE_TIFF_MM && ImageInfo->FileType!=IMAGE_FILETYPE_JPEG)) {
 				if (value_ptr < dir_entry) {
 					 
 					 
 					exif_error_docref("exif_read_data#error_ifd" EXIFERR_CC, ImageInfo, E_WARNING, "Process tag(x%04X=%s): Illegal pointer offset(x%04X < x%04X)", tag, exif_get_tagname(tag, tagname, -12, tag_table TSRMLS_CC), offset_val, dir_entry);
 				} else {
 					 
 					 
 					exif_error_docref("exif_read_data#error_ifd" EXIFERR_CC, ImageInfo, E_WARNING, "Process tag(x%04X=%s): Illegal pointer offset(x%04X + x%04X = x%04X > x%04X)", tag, exif_get_tagname(tag, tagname, -12, tag_table TSRMLS_CC), offset_val, byte_count, offset_val+byte_count, IFDlength);
 				}
 				return FALSE;
 			}
 			if (byte_count>sizeof(cbuf)) {
 				 
 				value_ptr = safe_emalloc(byte_count, 1, 0);
 				outside = value_ptr;
 			} else {
 				 
 				memset(&cbuf, 0, sizeof(cbuf));
 				value_ptr = cbuf;
 			}
 
 			fpos = php_stream_tell(ImageInfo->infile);
 			php_stream_seek(ImageInfo->infile, offset_val, SEEK_SET);
 			fgot = php_stream_tell(ImageInfo->infile);
 			if (fgot!=offset_val) {
 				EFREE_IF(outside);
 				exif_error_docref(NULL EXIFERR_CC, ImageInfo, E_WARNING, "Wrong file pointer: 0x%08X != 0x%08X", fgot, offset_val);
 				return FALSE;
 			}
 			fgot = php_stream_read(ImageInfo->infile, value_ptr, byte_count);
 			php_stream_seek(ImageInfo->infile, fpos, SEEK_SET);
 			if (fgot<byte_count) {
 				EFREE_IF(outside);
 				EXIF_ERRLOG_FILEEOF(ImageInfo)
 				return FALSE;
 			}
 		}
 	} else {
 		 
 		value_ptr = dir_entry+8;
 		offset_val= value_ptr-offset_base;
 	}
 
 	ImageInfo->sections_found |= FOUND_ANY_TAG;
 #ifdef EXIF_DEBUG
 	dump_data = exif_dump_data(&dump_free, format, components, length, ImageInfo->motorola_intel, value_ptr TSRMLS_CC);
 	exif_error_docref(NULL EXIFERR_CC, ImageInfo, E_NOTICE, "Process tag(x%04X=%s,@x%04X + x%04X(=%d)): %s%s %s", tag, exif_get_tagname(tag, tagname, -12, tag_table TSRMLS_CC), offset_val+displacement, byte_count, byte_count, (components>1)&&format!=TAG_FMT_UNDEFINED&&format!=TAG_FMT_STRING?"ARRAY OF ":"", exif_get_tagformat(format), dump_data);
 	if (dump_free) {
 		efree(dump_data);
 	}
 #endif
 
 	if (section_index==SECTION_THUMBNAIL) {
 		if (!ImageInfo->Thumbnail.data) {
 			switch(tag) {
 				case TAG_IMAGEWIDTH:
 				case TAG_COMP_IMAGE_WIDTH:
 					ImageInfo->Thumbnail.width = exif_convert_any_to_int(value_ptr, format, ImageInfo->motorola_intel TSRMLS_CC);
 					break;
 
 				case TAG_IMAGEHEIGHT:
 				case TAG_COMP_IMAGE_HEIGHT:
 					ImageInfo->Thumbnail.height = exif_convert_any_to_int(value_ptr, format, ImageInfo->motorola_intel TSRMLS_CC);
 					break;
 
 				case TAG_STRIP_OFFSETS:
 				case TAG_JPEG_INTERCHANGE_FORMAT:
 					 
 					ImageInfo->Thumbnail.offset = exif_convert_any_to_int(value_ptr, format, ImageInfo->motorola_intel TSRMLS_CC);
 					break;
 
 				case TAG_STRIP_BYTE_COUNTS:
 					if (ImageInfo->FileType == IMAGE_FILETYPE_TIFF_II || ImageInfo->FileType == IMAGE_FILETYPE_TIFF_MM) {
 						ImageInfo->Thumbnail.filetype = ImageInfo->FileType;
 					} else {
 						 
 						ImageInfo->Thumbnail.filetype = IMAGE_FILETYPE_TIFF_MM;
 					}
 					ImageInfo->Thumbnail.size = exif_convert_any_to_int(value_ptr, format, ImageInfo->motorola_intel TSRMLS_CC);
 					break;
 
 				case TAG_JPEG_INTERCHANGE_FORMAT_LEN:
 					if (ImageInfo->Thumbnail.filetype == IMAGE_FILETYPE_UNKNOWN) {
 						ImageInfo->Thumbnail.filetype = IMAGE_FILETYPE_JPEG;
 						ImageInfo->Thumbnail.size = exif_convert_any_to_int(value_ptr, format, ImageInfo->motorola_intel TSRMLS_CC);
 					}
 					break;
 			}
 		}
 	} else {
 		if (section_index==SECTION_IFD0 || section_index==SECTION_EXIF)
 		switch(tag) {
 			case TAG_COPYRIGHT:
 				 
 				if (byte_count>1 && (length=php_strnlen(value_ptr, byte_count)) > 0) {
 					if (length<byte_count-1) {
                                                  
                                                 ImageInfo->CopyrightPhotographer  = estrdup(value_ptr);
                                                 ImageInfo->CopyrightEditor        = estrndup(value_ptr+length+1, byte_count-length-1);
                                               spprintf(&ImageInfo->Copyright, 0, "%s, %s", value_ptr, value_ptr+length+1);
//                                                spprintf(&ImageInfo->Copyright, 0, "%s, %s", ImageInfo->CopyrightPhotographer, ImageInfo->CopyrightEditor);
                                                  
                                                  
                                                  
 					} else {
 						ImageInfo->Copyright = estrndup(value_ptr, byte_count);
 					}
 				}
 				break;
 
 			case TAG_USERCOMMENT:
 				ImageInfo->UserCommentLength = exif_process_user_comment(ImageInfo, &(ImageInfo->UserComment), &(ImageInfo->UserCommentEncoding), value_ptr, byte_count TSRMLS_CC);
 				break;
 
 			case TAG_XP_TITLE:
 			case TAG_XP_COMMENTS:
 			case TAG_XP_AUTHOR:
 			case TAG_XP_KEYWORDS:
 			case TAG_XP_SUBJECT:
 				tmp_xp = (xp_field_type*)safe_erealloc(ImageInfo->xp_fields.list, (ImageInfo->xp_fields.count+1), sizeof(xp_field_type), 0);
 				ImageInfo->sections_found |= FOUND_WINXP;
 				ImageInfo->xp_fields.list = tmp_xp;
 				ImageInfo->xp_fields.count++;
 				exif_process_unicode(ImageInfo, &(ImageInfo->xp_fields.list[ImageInfo->xp_fields.count-1]), tag, value_ptr, byte_count TSRMLS_CC);
 				break;
 
 			case TAG_FNUMBER:
 				 
 				ImageInfo->ApertureFNumber = (float)exif_convert_any_format(value_ptr, format, ImageInfo->motorola_intel TSRMLS_CC);
 				break;
 
 			case TAG_APERTURE:
 			case TAG_MAX_APERTURE:
 				 
 				if (ImageInfo->ApertureFNumber == 0) {
 					ImageInfo->ApertureFNumber
 						= (float)exp(exif_convert_any_format(value_ptr, format, ImageInfo->motorola_intel TSRMLS_CC)*log(2)*0.5);
 				}
 				break;
 
 			case TAG_SHUTTERSPEED:
 				 
 				if (ImageInfo->ExposureTime == 0) {
 					ImageInfo->ExposureTime
 						= (float)(1/exp(exif_convert_any_format(value_ptr, format, ImageInfo->motorola_intel TSRMLS_CC)*log(2)));
 				}
 				break;
 			case TAG_EXPOSURETIME:
 				ImageInfo->ExposureTime = -1;
 				break;
 
 			case TAG_COMP_IMAGE_WIDTH:
 				ImageInfo->ExifImageWidth = exif_convert_any_to_int(value_ptr, format, ImageInfo->motorola_intel TSRMLS_CC);
 				break;
 
 			case TAG_FOCALPLANE_X_RES:
 				ImageInfo->FocalplaneXRes = exif_convert_any_format(value_ptr, format, ImageInfo->motorola_intel TSRMLS_CC);
 				break;
 
 			case TAG_SUBJECT_DISTANCE:
 				 
 				ImageInfo->Distance = (float)exif_convert_any_format(value_ptr, format, ImageInfo->motorola_intel TSRMLS_CC);
 				break;
 
 			case TAG_FOCALPLANE_RESOLUTION_UNIT:
 				switch((int)exif_convert_any_format(value_ptr, format, ImageInfo->motorola_intel TSRMLS_CC)) {
 					case 1: ImageInfo->FocalplaneUnits = 25.4; break;  
 					case 2:
 						 
 						ImageInfo->FocalplaneUnits = 25.4;
 						break;
 
 					case 3: ImageInfo->FocalplaneUnits = 10;   break;   
 					case 4: ImageInfo->FocalplaneUnits = 1;    break;   
 					case 5: ImageInfo->FocalplaneUnits = .001; break;   
 				}
 				break;
 
 			case TAG_SUB_IFD:
 				if (format==TAG_FMT_IFD) {
 					 
 					 
 					 
 					exif_error_docref(NULL EXIFERR_CC, ImageInfo, E_NOTICE, "Skip SUB IFD");
 				}
 				break;
 
 			case TAG_MAKE:
 				ImageInfo->make = estrndup(value_ptr, byte_count);
 				break;
 			case TAG_MODEL:
 				ImageInfo->model = estrndup(value_ptr, byte_count);
 				break;
 
 			case TAG_MAKER_NOTE:
 				exif_process_IFD_in_MAKERNOTE(ImageInfo, value_ptr, byte_count, offset_base, IFDlength, displacement TSRMLS_CC);
 				break;
 
 			case TAG_EXIF_IFD_POINTER:
 			case TAG_GPS_IFD_POINTER:
 			case TAG_INTEROP_IFD_POINTER:
 				if (ReadNextIFD) {
 					char *Subdir_start;
 					int sub_section_index = 0;
 					switch(tag) {
 						case TAG_EXIF_IFD_POINTER:
 #ifdef EXIF_DEBUG
 							exif_error_docref(NULL EXIFERR_CC, ImageInfo, E_NOTICE, "Found EXIF");
 #endif
 							ImageInfo->sections_found |= FOUND_EXIF;
 							sub_section_index = SECTION_EXIF;
 							break;
 						case TAG_GPS_IFD_POINTER:
 #ifdef EXIF_DEBUG
 							exif_error_docref(NULL EXIFERR_CC, ImageInfo, E_NOTICE, "Found GPS");
 #endif
 							ImageInfo->sections_found |= FOUND_GPS;
 							sub_section_index = SECTION_GPS;
 							break;
 						case TAG_INTEROP_IFD_POINTER:
 #ifdef EXIF_DEBUG
 							exif_error_docref(NULL EXIFERR_CC, ImageInfo, E_NOTICE, "Found INTEROPERABILITY");
 #endif
 							ImageInfo->sections_found |= FOUND_INTEROP;
 							sub_section_index = SECTION_INTEROP;
 							break;
 					}
 					Subdir_start = offset_base + php_ifd_get32u(value_ptr, ImageInfo->motorola_intel);
 					if (Subdir_start < offset_base || Subdir_start > offset_base+IFDlength) {
 						exif_error_docref("exif_read_data#error_ifd" EXIFERR_CC, ImageInfo, E_WARNING, "Illegal IFD Pointer");
 						return FALSE;
 					}
 					if (!exif_process_IFD_in_JPEG(ImageInfo, Subdir_start, offset_base, IFDlength, displacement, sub_section_index TSRMLS_CC)) {
 						return FALSE;
 					}
 #ifdef EXIF_DEBUG
 					exif_error_docref(NULL EXIFERR_CC, ImageInfo, E_NOTICE, "Subsection %s done", exif_get_sectionname(sub_section_index));
 #endif
 				}
 		}
 	}
 	exif_iif_add_tag(ImageInfo, section_index, exif_get_tagname(tag, tagname, sizeof(tagname), tag_table TSRMLS_CC), tag, format, components, value_ptr TSRMLS_CC);
 	EFREE_IF(outside);
 	return TRUE;
 }