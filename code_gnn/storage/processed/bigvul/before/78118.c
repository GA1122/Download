static void print_hex(const u8 * buf, size_t buflen, size_t depth)
{
	size_t lines_len = buflen * 5 + 128;
	char *lines = malloc(lines_len);
	char *line = lines;

	if (buf == NULL || buflen == 0 || lines == NULL) {
		free(lines);
		return;
	}

	sc_hex_dump(buf, buflen, lines, lines_len);

	while (*line != '\0') {
		char *line_end = strchr(line, '\n');
		ptrdiff_t width = line_end - line;
		if (!line_end || width <= 1) {
			 
			break;
		}
		if (buflen > 8) {
			putchar('\n');
			print_indent(depth);
		} else {
			printf(": ");
		}
		printf("%.*s", (int) width, line);
		line = line_end + 1;
	}

	free(lines);
}