ukm::UkmRecorder* Document::UkmRecorder() {
  if (ukm_recorder_)
    return ukm_recorder_.get();

  ukm::mojom::UkmRecorderInterfacePtr interface;
  Platform::Current()->GetInterfaceProvider()->GetInterface(
      mojo::MakeRequest(&interface));
  ukm_recorder_.reset(new ukm::MojoUkmRecorder(std::move(interface)));
  ukm_source_id_ = ukm_recorder_->GetNewSourceID();
  ukm_recorder_->UpdateSourceURL(ukm_source_id_, url_);
  return ukm_recorder_.get();
}
