bool StructTraits<bluetooth::mojom::UUIDDataView, WTF::String>::Read(
    bluetooth::mojom::UUIDDataView data,
    WTF::String* output) {
   return data.ReadUuid(output);
 }
