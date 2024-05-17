void WebTransformOperations::appendRotate(double x, double y, double z, double degrees)
{
    WebTransformOperation toAdd;
    toAdd.matrix.rotate3d(x, y, z, degrees);
    toAdd.type = WebTransformOperation::WebTransformOperationRotate;
    toAdd.rotate.axis.x = x;
    toAdd.rotate.axis.y = y;
    toAdd.rotate.axis.z = z;
    toAdd.rotate.angle = degrees;
    m_private->operations.append(toAdd);
}
