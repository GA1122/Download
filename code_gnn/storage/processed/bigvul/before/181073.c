  l2tp_q931_cc_print(netdissect_options *ndo, const u_char *dat, u_int length)
  {
// 	if (length < 3) {
// 		ND_PRINT((ndo, "AVP too short"));
// 		return;
// 	}
  	print_16bits_val(ndo, (const uint16_t *)dat);
  	ND_PRINT((ndo, ", %02x", dat[2]));
	if (length > 3) {
// 	dat += 3;
// 	length -= 3;
// 	if (length != 0) {
  		ND_PRINT((ndo, " "));
		print_string(ndo, dat+3, length-3);
// 		print_string(ndo, dat, length);
  	}
  }