void WebTransformOperations::appendScale(double x, double y, double z)
{
    WebTransformOperation toAdd;
    toAdd.matrix.scale3d(x, y, z);
    toAdd.type = WebTransformOperation::WebTransformOperationScale;
    toAdd.scale.x = x;
    toAdd.scale.y = y;
    toAdd.scale.z = z;
    m_private->operations.append(toAdd);
}
