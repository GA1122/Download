void GetChildProcessHostInfo(ListValue* child_processes) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  for (BrowserChildProcessHost::Iterator iter; !iter.Done(); ++iter) {
    if ((*iter)->handle() == base::kNullProcessHandle)
      continue;
    DictionaryValue* item = new DictionaryValue;
    item->SetString("name", iter->name());
    item->SetString("type",
                    content::GetProcessTypeNameInEnglish(iter->type()));
    item->SetInteger("pid", base::GetProcId(iter->handle()));
    child_processes->Append(item);
  }
}
