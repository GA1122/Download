static PassRefPtr<JSONArray> buildArrayForQuad(const FloatQuad& quad)
{
    RefPtr<JSONArray> array = JSONArray::create();
    array->pushObject(buildObjectForPoint(quad.p1()));
    array->pushObject(buildObjectForPoint(quad.p2()));
    array->pushObject(buildObjectForPoint(quad.p3()));
    array->pushObject(buildObjectForPoint(quad.p4()));
    return array.release();
}
