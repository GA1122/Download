q933_print(netdissect_options *ndo,
           const u_char *p, u_int length)
{
	u_int olen;
	u_int call_ref_length, i;
	uint8_t call_ref[15];	 
	u_int msgtype;
	u_int iecode;
	u_int ielength;
	u_int codeset = 0;
	u_int is_ansi = 0;
	u_int ie_is_known;
	u_int non_locking_shift;
	u_int unshift_codeset;

	ND_PRINT((ndo, "%s", ndo->ndo_eflag ? "" : "Q.933"));

	if (length == 0 || !ND_TTEST(*p)) {
		if (!ndo->ndo_eflag)
			ND_PRINT((ndo, ", "));
		ND_PRINT((ndo, "length %u", length));
		goto trunc;
	}

	 
	olen = length;  
	call_ref_length = (*p) & 0x0f;
	p++;
	length--;

	 
	for (i = 0; i < call_ref_length; i++) {
		if (length == 0 || !ND_TTEST(*p)) {
			if (!ndo->ndo_eflag)
				ND_PRINT((ndo, ", "));
			ND_PRINT((ndo, "length %u", olen));
			goto trunc;
		}
		call_ref[i] = *p;
		p++;
		length--;
	}

	 
	if (length == 0 || !ND_TTEST(*p)) {
		if (!ndo->ndo_eflag)
			ND_PRINT((ndo, ", "));
		ND_PRINT((ndo, "length %u", olen));
		goto trunc;
	}
	msgtype = *p;
	p++;
	length--;

	 
	non_locking_shift = 0;
	unshift_codeset = codeset;
	if (length != 0) {
		if (!ND_TTEST(*p)) {
			if (!ndo->ndo_eflag)
				ND_PRINT((ndo, ", "));
			ND_PRINT((ndo, "length %u", olen));
			goto trunc;
		}
		iecode = *p;
		if (IE_IS_SHIFT(iecode)) {
			 
			p++;
			length--;

			 
			codeset = IE_SHIFT_CODESET(iecode);

			 
			if (IE_SHIFT_IS_LOCKING(iecode)) {
				 
				if (codeset == 5) {
					 
					is_ansi = 1;
				}
			} else {
				 
				non_locking_shift = 1;
				unshift_codeset = 0;
			}
		}
	}

	 
	if (!ndo->ndo_eflag)
		ND_PRINT((ndo, ", "));
	ND_PRINT((ndo, "%s, codeset %u", is_ansi ? "ANSI" : "CCITT", codeset));

	if (call_ref_length != 0) {
		ND_TCHECK(p[0]);
		if (call_ref_length > 1 || p[0] != 0) {
			 
			ND_PRINT((ndo, ", Call Ref: 0x"));
			for (i = 0; i < call_ref_length; i++)
				ND_PRINT((ndo, "%02x", call_ref[i]));
		}
	}
	if (ndo->ndo_vflag) {
		ND_PRINT((ndo, ", %s (0x%02x), length %u",
		   tok2str(fr_q933_msg_values,
			"unknown message", msgtype),
		   msgtype,
		   olen));
	} else {
		ND_PRINT((ndo, ", %s",
		       tok2str(fr_q933_msg_values,
			       "unknown message 0x%02x", msgtype)));
	}

	 
	while (length != 0) {
		 
		if (non_locking_shift == 1) {
			 
			non_locking_shift = 2;
		} else if (non_locking_shift == 2) {
			 
			codeset = unshift_codeset;
			non_locking_shift = 0;
		}

		 
		if (!ND_TTEST(*p)) {
			if (!ndo->ndo_vflag) {
				ND_PRINT((ndo, ", length %u", olen));
			}
			goto trunc;
		}
		iecode = *p;
		p++;
		length--;

		 
		if (IE_IS_SINGLE_OCTET(iecode)) {
			 
			if (IE_IS_SHIFT(iecode)) {
				 
				if (IE_SHIFT_IS_LOCKING(iecode)) {
					 
					non_locking_shift = 0;
				} else {
					 
					non_locking_shift = 1;
					unshift_codeset = codeset;
				}

				 
				codeset = IE_SHIFT_CODESET(iecode);
			}
		} else {
			 
			if (length == 0 || !ND_TTEST(*p)) {
				if (!ndo->ndo_vflag) {
					ND_PRINT((ndo, ", length %u", olen));
				}
				goto trunc;
			}
			ielength = *p;
			p++;
			length--;

			 
			if (ndo->ndo_vflag) {
				ND_PRINT((ndo, "\n\t%s IE (0x%02x), length %u: ",
				    tok2str(fr_q933_ie_codesets[codeset],
					"unknown", iecode),
				    iecode,
				    ielength));
			}

			 
			if (iecode == 0 || ielength == 0) {
				return;
			}
			if (length < ielength || !ND_TTEST2(*p, ielength)) {
				if (!ndo->ndo_vflag) {
					ND_PRINT((ndo, ", length %u", olen));
				}
				goto trunc;
			}

			ie_is_known = 0;
			if (fr_q933_print_ie_codeset[codeset] != NULL) {
				ie_is_known = fr_q933_print_ie_codeset[codeset](ndo, iecode, ielength, p);
			}

			if (ie_is_known) {
				 
				if (ndo->ndo_vflag > 1) {
					 
					print_unknown_data(ndo, p, "\n\t  ", ielength);
				}
			} else {
				 
				if (ndo->ndo_vflag >= 1) {
					print_unknown_data(ndo, p, "\n\t", ielength);
				}
			}

			length -= ielength;
			p += ielength;
		}
	}
	if (!ndo->ndo_vflag) {
	    ND_PRINT((ndo, ", length %u", olen));
	}
	return;

trunc:
	ND_PRINT((ndo, "[|q.933]"));
}
