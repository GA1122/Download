 void WebTransformOperations::appendTranslate(double x, double y, double z)
 {
     WebTransformOperation toAdd;
    toAdd.matrix.translate3d(x, y, z);
    toAdd.type = WebTransformOperation::WebTransformOperationTranslate;
    toAdd.translate.x = x;
    toAdd.translate.y = y;
    toAdd.translate.z = z;
    m_private->operations.append(toAdd);
}
