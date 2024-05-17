FileTransfer::InsertPluginMappings(MyString methods, MyString p)
{
	StringList method_list(methods.Value());

	char* m;

	method_list.rewind();
	while((m = method_list.next())) {
		dprintf(D_FULLDEBUG, "FILETRANSFER: protocol \"%s\" handled by \"%s\"\n", m, p.Value());
		plugin_table->insert(m, p);
	}
}
