static float* jsArrayToFloatArray(v8::Handle<v8::Array> array, uint32_t len)
{
    float* data = 0;
    if (len > std::numeric_limits<uint32_t>::max() / sizeof(float)
        || !tryFastMalloc(len * sizeof(float)).getValue(data))
        return 0;
    for (uint32_t i = 0; i < len; i++) {
        v8::Local<v8::Value> val = array->Get(i);
        if (!val->IsNumber()) {
            fastFree(data);
            return 0;
        }
        data[i] = toFloat(val);
    }
    return data;
}
