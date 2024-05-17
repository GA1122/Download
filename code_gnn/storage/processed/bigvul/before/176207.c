void CheckArrayAbuse(Handle<JSObject> obj, const char* op, uint32_t index,
 bool allow_appending) {
 DisallowHeapAllocation no_allocation;
 Object* raw_length = NULL;
 const char* elements_type = "array";
 if (obj->IsJSArray()) {
 JSArray* array = JSArray::cast(*obj);
    raw_length = array->length();
 } else {
    raw_length = Smi::FromInt(obj->elements()->length());
    elements_type = "object";
 }

 if (raw_length->IsNumber()) {
 double n = raw_length->Number();
 if (FastI2D(FastD2UI(n)) == n) {
 int32_t int32_length = DoubleToInt32(n);
 uint32_t compare_length = static_cast<uint32_t>(int32_length);
 if (allow_appending) compare_length++;
 if (index >= compare_length) {
 PrintF("[OOB %s %s (%s length = %d, element accessed = %d) in ",
               elements_type, op, elements_type, static_cast<int>(int32_length),
 static_cast<int>(index));
 TraceTopFrame(obj->GetIsolate());
 PrintF("]\n");
 }
 } else {
 PrintF("[%s elements length not integer value in ", elements_type);
 TraceTopFrame(obj->GetIsolate());
 PrintF("]\n");
 }
 } else {
 PrintF("[%s elements length not a number in ", elements_type);
 TraceTopFrame(obj->GetIsolate());
 PrintF("]\n");
 }
}
