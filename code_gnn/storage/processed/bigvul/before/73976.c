fd_readline (int fdi, int fdo, char *b, int bsz)
{
	int r;
	unsigned char c;
	unsigned char *bp, *bpe;
	
	bp = (unsigned char *)b;
	bpe = (unsigned char *)b + bsz - 1;

	while (1) {
		r = read(fdi, &c, 1);
		if ( r <= 0 ) { r = -1; goto out; }

		switch (c) {
		case '\b':
		case '\x7f':
			if ( bp > (unsigned char *)b ) { 
				bp--;
				cput(fdo, '\b'); cput(fdo, ' '); cput(fdo, '\b');
			} else {
				cput(fdo, '\x07');
			}
			break;
		case '\x03':  
			r = -1;
			errno = EINTR;
			goto out;
		case '\r':
			*bp = '\0';
			r = bp - (unsigned char *)b; 
			goto out;
		default:
			if ( bp < bpe ) { *bp++ = c; cput(fdo, c); }
			else { cput(fdo, '\x07'); }
			break;
		}
	}

out:
	return r;
}