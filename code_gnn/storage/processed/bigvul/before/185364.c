 void BiquadDSPKernel::getFrequencyResponse(int nFrequencies,
                                            const float* frequencyHz,
                                            float* magResponse,
                                            float* phaseResponse)
 {
     bool isGood = nFrequencies > 0 && frequencyHz && magResponse && phaseResponse;
     ASSERT(isGood);
     if (!isGood)
         return;
 
     Vector<float> frequency(nFrequencies);
 
     double nyquist = this->nyquist();
 
      for (int k = 0; k < nFrequencies; ++k)
          frequency[k] = narrowPrecisionToFloat(frequencyHz[k] / nyquist);
  
//     double cutoffFrequency;
//     double Q;
//     double gain;
//     double detune;  
// 
//     {
//          
//          
//          
//          
//          
//          
//          
//          
//          
//          
//          
//          
//         MutexLocker processLocker(m_processLock);
// 
//         cutoffFrequency = biquadProcessor()->parameter1()->value();
//         Q = biquadProcessor()->parameter2()->value();
//         gain = biquadProcessor()->parameter3()->value();
//         detune = biquadProcessor()->parameter4()->value();
//     }
  
    updateCoefficientsIfNecessary(false, true);
//     updateCoefficients(cutoffFrequency, Q, gain, detune);
  
      m_biquad.getFrequencyResponse(nFrequencies, frequency.data(), magResponse, phaseResponse);
  }