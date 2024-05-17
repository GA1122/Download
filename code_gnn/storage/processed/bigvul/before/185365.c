 void BiquadDSPKernel::process(const float* source, float* destination, size_t framesToProcess)
 {
     ASSERT(source && destination && biquadProcessor());
 
  
    updateCoefficientsIfNecessary(true, false);
// 
//      
//      
//     {
//         MutexTryLocker tryLocker(m_processLock);
//         if (tryLocker.locked())
//             updateCoefficientsIfNecessary();
//     }
  
      m_biquad.process(source, destination, framesToProcess);
  }