FontFileAddEntry(FontTablePtr table, FontEntryPtr prototype)
{
    FontEntryPtr    entry;
    int		    newsize;

     
    if (table->sorted)
	return (FontEntryPtr) 0;     
    if (table->used == table->size) {
	if (table->size >= ((INT32_MAX / sizeof(FontEntryRec)) - 100))
	     
	    return NULL;
	newsize = table->size + 100;
	entry = realloc(table->entries, newsize * sizeof(FontEntryRec));
	if (!entry)
	    return (FontEntryPtr)0;
	table->size = newsize;
	table->entries = entry;
    }
    entry = &table->entries[table->used];
    *entry = *prototype;
    entry->name.name = malloc(prototype->name.length + 1);
    if (!entry->name.name)
	return (FontEntryPtr)0;
    memcpy (entry->name.name, prototype->name.name, prototype->name.length);
    entry->name.name[entry->name.length] = '\0';
    table->used++;
    return entry;
}