JSValue JSFloat64Array::getByIndex(ExecState*, unsigned index)
{
    ASSERT_GC_OBJECT_INHERITS(this, &s_info);
    double result = static_cast<Float64Array*>(impl())->item(index);
    if (isnan(result))
        return jsNaN();
    return JSValue(result);
}
