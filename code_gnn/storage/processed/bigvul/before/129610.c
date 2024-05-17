SubtreeContentTransformScope::SubtreeContentTransformScope(const AffineTransform& subtreeContentTransformation)
    : m_savedContentTransformation(s_currentContentTransformation)
{
    AffineTransform contentTransformation = subtreeContentTransformation * AffineTransform(s_currentContentTransformation);
    contentTransformation.copyTransformTo(s_currentContentTransformation);
}
