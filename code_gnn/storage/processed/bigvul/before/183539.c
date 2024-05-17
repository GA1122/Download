 void Vp9Parser::ReadSegmentationMap() {
// void Vp9Parser::ReadSegmentationMap(Vp9Segmentation* segment) {
    for (size_t i = 0; i < Vp9Segmentation::kNumTreeProbs; i++) {
    segmentation_.tree_probs[i] =
//     segment->tree_probs[i] =
          reader_.ReadBool() ? reader_.ReadLiteral(8) : kVp9MaxProb;
    }
  
    for (size_t i = 0; i < Vp9Segmentation::kNumPredictionProbs; i++)
    segmentation_.pred_probs[i] = kVp9MaxProb;
//     segment->pred_probs[i] = kVp9MaxProb;
  
  segmentation_.temporal_update = reader_.ReadBool();
  if (segmentation_.temporal_update) {
//   segment->temporal_update = reader_.ReadBool();
//   if (segment->temporal_update) {
      for (size_t i = 0; i < Vp9Segmentation::kNumPredictionProbs; i++) {
        if (reader_.ReadBool())
        segmentation_.pred_probs[i] = reader_.ReadLiteral(8);
//         segment->pred_probs[i] = reader_.ReadLiteral(8);
      }
    }
  }