ikev2_e_print(netdissect_options *ndo,
#ifndef HAVE_LIBCRYPTO
	      _U_
#endif
	      struct isakmp *base,
	      u_char tpay,
	      const struct isakmp_gen *ext,
	      u_int item_len _U_, const u_char *ep _U_,
#ifndef HAVE_LIBCRYPTO
	      _U_
#endif
	      uint32_t phase,
#ifndef HAVE_LIBCRYPTO
	      _U_
#endif
	      uint32_t doi,
#ifndef HAVE_LIBCRYPTO
	      _U_
#endif
	      uint32_t proto,
#ifndef HAVE_LIBCRYPTO
	      _U_
#endif
	      int depth)
{
	struct isakmp_gen e;
	const u_char *dat;
	volatile int dlen;

	ND_TCHECK(*ext);
	UNALIGNED_MEMCPY(&e, ext, sizeof(e));
	ikev2_pay_print(ndo, NPSTR(tpay), e.critical);

	dlen = ntohs(e.len)-4;

	ND_PRINT((ndo," len=%d", dlen));
	if (2 < ndo->ndo_vflag && 4 < dlen) {
		ND_PRINT((ndo," "));
		if (!rawprint(ndo, (const uint8_t *)(ext + 1), dlen))
			goto trunc;
	}

	dat = (const u_char *)(ext+1);
	ND_TCHECK2(*dat, dlen);

#ifdef HAVE_LIBCRYPTO
	 
	if(esp_print_decrypt_buffer_by_ikev2(ndo,
					     base->flags & ISAKMP_FLAG_I,
					     base->i_ck, base->r_ck,
					     dat, dat+dlen)) {

		ext = (const struct isakmp_gen *)ndo->ndo_packetp;

		 
		ikev2_sub_print(ndo, base, e.np, ext, ndo->ndo_snapend,
				phase, doi, proto, depth+1);
	}
#endif


	 
	return NULL;
trunc:
	ND_PRINT((ndo," [|%s]", NPSTR(tpay)));
	return NULL;
}