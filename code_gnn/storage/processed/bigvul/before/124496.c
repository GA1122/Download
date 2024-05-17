static void appendLayers(Vector<ImageResource*>& images, const FillLayer* styleLayer)
{
    for (const FillLayer* layer = styleLayer; layer; layer = layer->next()) {
        appendImageIfNotNull(images, layer->image());
    }
}
