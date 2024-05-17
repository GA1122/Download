static void safe_print(const char *cp, int len, const char *esc)
{
	unsigned char	ch;

	if (len < 0)
		len = strlen(cp);

	while (len--) {
		ch = *cp++;
		if (!raw_chars) {
			if (ch >= 128) {
				fputs("M-", stdout);
				ch -= 128;
			}
			if ((ch < 32) || (ch == 0x7f)) {
				fputc('^', stdout);
				ch ^= 0x40;  

			} else if (esc && strchr(esc, ch))
				fputc('\\', stdout);
		}
		fputc(ch, stdout);
	}
}