AffineTransform SVGLayoutSupport::deprecatedCalculateTransformToLayer(const LayoutObject* layoutObject)
{
    AffineTransform transform;
    while (layoutObject) {
        transform = layoutObject->localToSVGParentTransform() * transform;
        if (layoutObject->isSVGRoot())
            break;
        layoutObject = layoutObject->parent();
    }

    PaintLayer* layer = layoutObject ? layoutObject->enclosingLayer() : 0;
    while (layer && layer->isAllowedToQueryCompositingState()) {
        if (layer->compositingState() != NotComposited)
            break;

        if (TransformationMatrix* layerTransform = layer->transform())
            transform = layerTransform->toAffineTransform() * transform;

        layer = layer->parent();
    }

    return transform;
}
