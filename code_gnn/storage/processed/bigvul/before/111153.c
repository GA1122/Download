static RenderLayer* findAncestorOrSelfNotMatching(PredicateFunction predicate, RenderLayer* layer)
{
    RenderLayer* curr = layer;
    while (curr && !predicate(curr))
        curr = curr->parent();

    return curr;
}
