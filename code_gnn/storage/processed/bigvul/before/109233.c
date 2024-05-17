bool InspectorOverlay::getBoxModel(Node* node, Vector<FloatQuad>* quads)
{
    return buildNodeQuads(node, *quads);
}
