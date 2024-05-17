AppendEntryToBuffer(Buffer *buffer, Entry *entry)
{
    AppendToBuffer(buffer, entry->tag, strlen(entry->tag));
    AppendToBuffer(buffer, ":\t", 2);
    AppendToBuffer(buffer, entry->value, strlen(entry->value));
    AppendToBuffer(buffer, "\n", 1);
}
