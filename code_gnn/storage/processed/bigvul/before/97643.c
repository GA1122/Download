xmlXPathDebugDumpLocationSet(FILE *output, xmlLocationSetPtr cur, int depth) {
    int i;
    char shift[100];

    for (i = 0;((i < depth) && (i < 25));i++)
        shift[2 * i] = shift[2 * i + 1] = ' ';
    shift[2 * i] = shift[2 * i + 1] = 0;

    if (cur == NULL) {
	fprintf(output, "%s", shift);
	fprintf(output, "LocationSet is NULL !\n");
	return;

    }

    for (i = 0;i < cur->locNr;i++) {
	fprintf(output, "%s", shift);
        fprintf(output, "%d : ", i + 1);
	xmlXPathDebugDumpObject(output, cur->locTab[i], depth + 1);
    }
}
