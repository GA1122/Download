 void RunTest() {
 {
      SCOPED_TRACE("DC_PRED");
 FillRandom();
 Predict(DC_PRED);
 CheckDCPrediction();
 }
 {
      SCOPED_TRACE("DC_PRED LEFT");
 FillRandom();
 SetLeftUnavailable();
 Predict(DC_PRED);
 CheckDCPrediction();
 }
 {
      SCOPED_TRACE("DC_PRED TOP");
 FillRandom();
 SetTopUnavailable();
 Predict(DC_PRED);
 CheckDCPrediction();
 }
 {
      SCOPED_TRACE("DC_PRED TOP_LEFT");
 FillRandom();
 SetTopLeftUnavailable();
 Predict(DC_PRED);
 CheckDCPrediction();
 }
 {
      SCOPED_TRACE("H_PRED");
 FillRandom();
 Predict(H_PRED);
 CheckHPrediction();
 }
 {
      SCOPED_TRACE("V_PRED");
 FillRandom();
 Predict(V_PRED);
 CheckVPrediction();
 }
 {
      SCOPED_TRACE("TM_PRED");
 FillRandom();
 Predict(TM_PRED);
 CheckTMPrediction();
 }
 }
