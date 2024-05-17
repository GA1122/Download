void WebTransformOperations::appendPerspective(double depth)
{
    WebTransformOperation toAdd;
    toAdd.matrix.applyPerspective(depth);
    toAdd.type = WebTransformOperation::WebTransformOperationPerspective;
    toAdd.perspectiveDepth = depth;
    m_private->operations.append(toAdd);
}
