    virtual PassRefPtr<GraphicsContext3D> createLayerTreeHostContext3D()
    {
        OwnPtr<WebGraphicsContext3D> mock = CompositorMockWebGraphicsContext3D::create();
        GraphicsContext3D::Attributes attrs;
        RefPtr<GraphicsContext3D> context = GraphicsContext3DPrivate::createGraphicsContextFromWebContext(mock.release(), attrs, 0, GraphicsContext3D::RenderDirectlyToHostWindow, GraphicsContext3DPrivate::ForUseOnAnotherThread);
        return context;
    }
