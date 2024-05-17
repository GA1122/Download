static const void* printBinderTransactionData(TextOutput& out, const void* data)
{
 const binder_transaction_data* btd =
 (const binder_transaction_data*)data;
 if (btd->target.handle < 1024) {
  
        out << "target.desc=" << btd->target.handle;
 } else {
        out << "target.ptr=" << btd->target.ptr;
 }
    out << " (cookie " << btd->cookie << ")" << endl
 << "code=" << TypeCode(btd->code) << ", flags=" << (void*)(long)btd->flags << endl
 << "data=" << btd->data.ptr.buffer << " (" << (void*)btd->data_size
 << " bytes)" << endl
 << "offsets=" << btd->data.ptr.offsets << " (" << (void*)btd->offsets_size
 << " bytes)";
 return btd+1;
}
