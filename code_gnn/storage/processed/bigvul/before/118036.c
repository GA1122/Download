static int* jsArrayToIntArray(v8::Handle<v8::Array> array, uint32_t len)
{
    int* data = 0;
    if (len > std::numeric_limits<uint32_t>::max() / sizeof(int)
        || !tryFastMalloc(len * sizeof(int)).getValue(data))
        return 0;
    for (uint32_t i = 0; i < len; i++) {
        v8::Local<v8::Value> val = array->Get(i);
        bool ok;
        int ival = toInt32(val, ok);
        if (!ok) {
            fastFree(data);
            return 0;
        }
        data[i] = ival;
    }
    return data;
}
