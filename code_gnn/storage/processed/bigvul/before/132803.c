PictureLayer::PictureLayer(ContentLayerClient* client,
                           scoped_ptr<RecordingSource> source)
    : PictureLayer(client) {
  recording_source_ = source.Pass();
}
