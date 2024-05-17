AddEntry(Entries *e, Entry *entry)
{
    register int n;

    for (n = 0; n < e->used; n++) {
	if (!strcmp(e->entry[n].tag, entry->tag)) {
	     
	    if (e->entry[n].lineno && !quiet) {
		fprintf (stderr, 
			 "%s:  \"%s\" on line %d overrides entry on line %d\n",
			 ProgramName, entry->tag, entry->lineno, 
			 e->entry[n].lineno);
	    }
	    free(e->entry[n].tag);
	    free(e->entry[n].value);
	    entry->usable = True;
	    e->entry[n] = *entry;
	    return;   
	}
    }

    if (e->used == e->room) {
	e->entry = (Entry *)realloc((char *)e->entry,
				    2*e->room*(sizeof(Entry)));
	e->room *= 2;
    }
    entry->usable = True;
    e->entry[e->used++] = *entry;
}
