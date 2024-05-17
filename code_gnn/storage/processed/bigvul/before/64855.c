static int decode_v4_header_fields(struct iwbmprcontext *rctx, const iw_byte *buf)
{
	int k;
	unsigned int cstype;

	if(rctx->uses_bitfields) {
		for(k=0;k<4;k++) {
			rctx->bf_mask[k] = 0;
		}
		for(k=0;k<4;k++) {
			if(rctx->infoheader_size < (size_t)(40+k*4+4)) break;
			rctx->bf_mask[k] = iw_get_ui32le(&buf[40+k*4]);
			if(!process_bf_mask(rctx,k)) return 0;
		}
		rctx->bitfields_set=1;  

		if(rctx->bf_mask[3]!=0) {
			rctx->has_alpha_channel = 1;
		}
	}

	if(rctx->infoheader_size < 108) return 1;

	cstype = iw_get_ui32le(&buf[56]);
	switch(cstype) {
	case IWBMPCS_CALIBRATED_RGB:
		break;

	case IWBMPCS_DEVICE_RGB:
	case IWBMPCS_SRGB:
	case IWBMPCS_WINDOWS:
		break;

	case IWBMPCS_PROFILE_LINKED:
	case IWBMPCS_PROFILE_EMBEDDED:
		if(rctx->bmpversion<5) {
			iw_warning(rctx->ctx,"Invalid colorspace type for BMPv4");
		}
		break;

	default:
		iw_warningf(rctx->ctx,"Unrecognized or unsupported colorspace type (0x%x)",cstype);
	}

	if(cstype==IWBMPCS_CALIBRATED_RGB) {
		unsigned int bmpgamma;
		double gamma[3];
		double avggamma;

		for(k=0;k<3;k++) {
			bmpgamma = iw_get_ui32le(&buf[96+k*4]);
			gamma[k] = ((double)bmpgamma)/65536.0;
		}
		avggamma = (gamma[0] + gamma[1] + gamma[2])/3.0;

		if(avggamma>=0.1 && avggamma<=10.0) {
			iw_make_gamma_csdescr(&rctx->csdescr,1.0/avggamma);
		}
	}

	return 1;
}
