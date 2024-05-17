SubtreeContentTransformScope::~SubtreeContentTransformScope()
{
    m_savedContentTransformation.copyTransformTo(s_currentContentTransformation);
}
