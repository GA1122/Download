init_decompression(struct archive_read *a, struct _7zip *zip,
    const struct _7z_coder *coder1, const struct _7z_coder *coder2)
{
	int r;

	zip->codec = coder1->codec;
	zip->codec2 = -1;

	switch (zip->codec) {
	case _7Z_COPY:
	case _7Z_BZ2:
	case _7Z_DEFLATE:
	case _7Z_PPMD:
		if (coder2 != NULL) {
			if (coder2->codec != _7Z_X86 &&
			    coder2->codec != _7Z_X86_BCJ2) {
				archive_set_error(&a->archive,
				    ARCHIVE_ERRNO_MISC,
				    "Unsupported filter %lx for %lx",
				    coder2->codec, coder1->codec);
				return (ARCHIVE_FAILED);
			}
			zip->codec2 = coder2->codec;
			zip->bcj_state = 0;
			if (coder2->codec == _7Z_X86)
				x86_Init(zip);
		}
		break;
	default:
		break;
	}

	switch (zip->codec) {
	case _7Z_COPY:
		break;

	case _7Z_LZMA: case _7Z_LZMA2:
#ifdef HAVE_LZMA_H
#if LZMA_VERSION_MAJOR >= 5
 
#define LZMA_MEMLIMIT   UINT64_MAX
#else
 
#define LZMA_MEMLIMIT   (1U << 30)
#endif
	{
		lzma_options_delta delta_opt;
		lzma_filter filters[LZMA_FILTERS_MAX], *ff;
		int fi = 0;

		if (zip->lzstream_valid) {
			lzma_end(&(zip->lzstream));
			zip->lzstream_valid = 0;
		}

		 
		if (coder2 != NULL) {
			zip->codec2 = coder2->codec;

			filters[fi].options = NULL;
			switch (zip->codec2) {
			case _7Z_X86:
				if (zip->codec == _7Z_LZMA2) {
					filters[fi].id = LZMA_FILTER_X86;
					fi++;
				} else
					 
					x86_Init(zip);
				break;
			case _7Z_X86_BCJ2:
				 
				zip->bcj_state = 0;
				break;
			case _7Z_DELTA:
				filters[fi].id = LZMA_FILTER_DELTA;
				memset(&delta_opt, 0, sizeof(delta_opt));
				delta_opt.type = LZMA_DELTA_TYPE_BYTE;
				delta_opt.dist = 1;
				filters[fi].options = &delta_opt;
				fi++;
				break;
			 
			case _7Z_POWERPC:
				filters[fi].id = LZMA_FILTER_POWERPC;
				fi++;
				break;
			case _7Z_IA64:
				filters[fi].id = LZMA_FILTER_IA64;
				fi++;
				break;
			case _7Z_ARM:
				filters[fi].id = LZMA_FILTER_ARM;
				fi++;
				break;
			case _7Z_ARMTHUMB:
				filters[fi].id = LZMA_FILTER_ARMTHUMB;
				fi++;
				break;
			case _7Z_SPARC:
				filters[fi].id = LZMA_FILTER_SPARC;
				fi++;
				break;
			default:
				archive_set_error(&a->archive,
				    ARCHIVE_ERRNO_MISC,
				    "Unexpected codec ID: %lX", zip->codec2);
				return (ARCHIVE_FAILED);
			}
		}

		if (zip->codec == _7Z_LZMA2)
			filters[fi].id = LZMA_FILTER_LZMA2;
		else
			filters[fi].id = LZMA_FILTER_LZMA1;
		filters[fi].options = NULL;
		ff = &filters[fi];
		r = lzma_properties_decode(&filters[fi], NULL,
		    coder1->properties, (size_t)coder1->propertiesSize);
		if (r != LZMA_OK) {
			set_error(a, r);
			return (ARCHIVE_FAILED);
		}
		fi++;

		filters[fi].id = LZMA_VLI_UNKNOWN;
		filters[fi].options = NULL;
		r = lzma_raw_decoder(&(zip->lzstream), filters);
		free(ff->options);
		if (r != LZMA_OK) {
			set_error(a, r);
			return (ARCHIVE_FAILED);
		}
		zip->lzstream_valid = 1;
		zip->lzstream.total_in = 0;
		zip->lzstream.total_out = 0;
		break;
	}
#else
		archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
		    "LZMA codec is unsupported");
		return (ARCHIVE_FAILED);
#endif
	case _7Z_BZ2:
#if defined(HAVE_BZLIB_H) && defined(BZ_CONFIG_ERROR)
		if (zip->bzstream_valid) {
			BZ2_bzDecompressEnd(&(zip->bzstream));
			zip->bzstream_valid = 0;
		}
		r = BZ2_bzDecompressInit(&(zip->bzstream), 0, 0);
		if (r == BZ_MEM_ERROR)
			r = BZ2_bzDecompressInit(&(zip->bzstream), 0, 1);
		if (r != BZ_OK) {
			int err = ARCHIVE_ERRNO_MISC;
			const char *detail = NULL;
			switch (r) {
			case BZ_PARAM_ERROR:
				detail = "invalid setup parameter";
				break;
			case BZ_MEM_ERROR:
				err = ENOMEM;
				detail = "out of memory";
				break;
			case BZ_CONFIG_ERROR:
				detail = "mis-compiled library";
				break;
			}
			archive_set_error(&a->archive, err,
			    "Internal error initializing decompressor: %s",
			    detail != NULL ? detail : "??");
			zip->bzstream_valid = 0;
			return (ARCHIVE_FAILED);
		}
		zip->bzstream_valid = 1;
		zip->bzstream.total_in_lo32 = 0;
		zip->bzstream.total_in_hi32 = 0;
		zip->bzstream.total_out_lo32 = 0;
		zip->bzstream.total_out_hi32 = 0;
		break;
#else
		archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
		    "BZ2 codec is unsupported");
		return (ARCHIVE_FAILED);
#endif
	case _7Z_DEFLATE:
#ifdef HAVE_ZLIB_H
		if (zip->stream_valid)
			r = inflateReset(&(zip->stream));
		else
			r = inflateInit2(&(zip->stream),
			    -15  );
		if (r != Z_OK) {
			archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
			    "Couldn't initialize zlib stream.");
			return (ARCHIVE_FAILED);
		}
		zip->stream_valid = 1;
		zip->stream.total_in = 0;
		zip->stream.total_out = 0;
		break;
#else
		archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
		    "DEFLATE codec is unsupported");
		return (ARCHIVE_FAILED);
#endif
	case _7Z_PPMD:
	{
		unsigned order;
		uint32_t msize;

		if (zip->ppmd7_valid) {
			__archive_ppmd7_functions.Ppmd7_Free(
			    &zip->ppmd7_context);
			zip->ppmd7_valid = 0;
		}

		if (coder1->propertiesSize < 5) {
			archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
			    "Malformed PPMd parameter");
			return (ARCHIVE_FAILED);
		}
		order = coder1->properties[0];
		msize = archive_le32dec(&(coder1->properties[1]));
		if (order < PPMD7_MIN_ORDER || order > PPMD7_MAX_ORDER ||
		    msize < PPMD7_MIN_MEM_SIZE || msize > PPMD7_MAX_MEM_SIZE) {
			archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
			    "Malformed PPMd parameter");
			return (ARCHIVE_FAILED);
		}
		__archive_ppmd7_functions.Ppmd7_Construct(&zip->ppmd7_context);
		r = __archive_ppmd7_functions.Ppmd7_Alloc(
			&zip->ppmd7_context, msize);
		if (r == 0) {
			archive_set_error(&a->archive, ENOMEM,
			    "Coludn't allocate memory for PPMd");
			return (ARCHIVE_FATAL);
		}
		__archive_ppmd7_functions.Ppmd7_Init(
			&zip->ppmd7_context, order);
		__archive_ppmd7_functions.Ppmd7z_RangeDec_CreateVTable(
			&zip->range_dec);
		zip->ppmd7_valid = 1;
		zip->ppmd7_stat = 0;
		zip->ppstream.overconsumed = 0;
		zip->ppstream.total_in = 0;
		zip->ppstream.total_out = 0;
		break;
	}
	case _7Z_X86:
	case _7Z_X86_BCJ2:
	case _7Z_POWERPC:
	case _7Z_IA64:
	case _7Z_ARM:
	case _7Z_ARMTHUMB:
	case _7Z_SPARC:
	case _7Z_DELTA:
		archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
		    "Unexpected codec ID: %lX", zip->codec);
		return (ARCHIVE_FAILED);
	case _7Z_CRYPTO_MAIN_ZIP:
	case _7Z_CRYPTO_RAR_29:
	case _7Z_CRYPTO_AES_256_SHA_256:
		if (a->entry) {
			archive_entry_set_is_metadata_encrypted(a->entry, 1);
			archive_entry_set_is_data_encrypted(a->entry, 1);
			zip->has_encrypted_entries = 1;
		}
		archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
		    "Crypto codec not supported yet (ID: 0x%lX)", zip->codec);
		return (ARCHIVE_FAILED);
	default:
		archive_set_error(&a->archive, ARCHIVE_ERRNO_MISC,
		    "Unknown codec ID: %lX", zip->codec);
		return (ARCHIVE_FAILED);
	}

	return (ARCHIVE_OK);
}
