Float64Array* toFloat64Array(JSC::JSValue value)
{
    return value.inherits(&JSFloat64Array::s_info) ? jsCast<JSFloat64Array*>(asObject(value))->impl() : 0;
}
