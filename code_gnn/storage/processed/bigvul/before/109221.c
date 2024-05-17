static PassRefPtr<JSONObject> buildObjectForPoint(const FloatPoint& point)
{
    RefPtr<JSONObject> object = JSONObject::create();
    object->setNumber("x", point.x());
    object->setNumber("y", point.y());
    return object.release();
}
