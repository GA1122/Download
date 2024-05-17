void Parcel::print(TextOutput& to, uint32_t  ) const
{
    to << "Parcel(";

 if (errorCheck() != NO_ERROR) {
 const status_t err = errorCheck();
        to << "Error: " << (void*)(intptr_t)err << " \"" << strerror(-err) << "\"";
 } else if (dataSize() > 0) {
 const uint8_t* DATA = data();
        to << indent << HexDump(DATA, dataSize()) << dedent;
 const binder_size_t* OBJS = objects();
 const size_t N = objectsCount();
 for (size_t i=0; i<N; i++) {
 const flat_binder_object* flat
 = reinterpret_cast<const flat_binder_object*>(DATA+OBJS[i]);
            to << endl << "Object #" << i << " @ " << (void*)OBJS[i] << ": "
 << TypeCode(flat->type & 0x7f7f7f00)
 << " = " << flat->binder;
 }
 } else {
        to << "NULL";
 }

    to << ")";
}
