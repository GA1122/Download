void Document::setTransformSource(std::unique_ptr<TransformSource> source)
{
    m_transformSource = std::move(source);
}
