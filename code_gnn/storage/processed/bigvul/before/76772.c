resolveEmphasisAllCapsSymbols(
		EmphasisInfo *buffer, formtype *typebuf, const InString *input) {
	 

	int inEmphasis = 0, i;

	for (i = 0; i < input->length; i++) {
		if (buffer[i].end & capsEmphClass) {
			inEmphasis = 0;
			buffer[i].end &= ~capsEmphClass;
		} else {
			if (buffer[i].begin & capsEmphClass) {
				buffer[i].begin &= ~capsEmphClass;
				inEmphasis = 1;
			}
			if (inEmphasis) {
				if (typebuf[i] & CAPSEMPH)
					 
					buffer[i].symbol |= capsEmphClass;
			}  
		}	  
	}
}
