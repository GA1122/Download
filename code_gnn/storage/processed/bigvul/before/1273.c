inline void Splash::pipeRun(SplashPipe *pipe) {
  Guchar aSrc, aDest, alpha2, alpha0, aResult;
  SplashColor cDest, cBlend;
  Guchar cResult0, cResult1, cResult2, cResult3;



  if (pipe->pattern) {
    pipe->pattern->getColor(pipe->x, pipe->y, pipe->cSrcVal);
  }

  if (pipe->noTransparency && !state->blendFunc) {


    switch (bitmap->mode) {
    case splashModeMono1:
      cResult0 = pipe->cSrc[0];
      if (state->screen->test(pipe->x, pipe->y, cResult0)) {
	*pipe->destColorPtr |= pipe->destColorMask;
      } else {
	*pipe->destColorPtr &= ~pipe->destColorMask;
      }
      if (!(pipe->destColorMask >>= 1)) {
	pipe->destColorMask = 0x80;
	++pipe->destColorPtr;
      }
      break;
    case splashModeMono8:
      *pipe->destColorPtr++ = pipe->cSrc[0];
      break;
    case splashModeRGB8:
      *pipe->destColorPtr++ = pipe->cSrc[0];
      *pipe->destColorPtr++ = pipe->cSrc[1];
      *pipe->destColorPtr++ = pipe->cSrc[2];
      break;
    case splashModeXBGR8:
      *pipe->destColorPtr++ = pipe->cSrc[2];
      *pipe->destColorPtr++ = pipe->cSrc[1];
      *pipe->destColorPtr++ = pipe->cSrc[0];
      *pipe->destColorPtr++ = 255;
      break;
    case splashModeBGR8:
      *pipe->destColorPtr++ = pipe->cSrc[2];
      *pipe->destColorPtr++ = pipe->cSrc[1];
      *pipe->destColorPtr++ = pipe->cSrc[0];
      break;
#if SPLASH_CMYK
    case splashModeCMYK8:
      *pipe->destColorPtr++ = pipe->cSrc[0];
      *pipe->destColorPtr++ = pipe->cSrc[1];
      *pipe->destColorPtr++ = pipe->cSrc[2];
      *pipe->destColorPtr++ = pipe->cSrc[3];
      break;
#endif
    }
    if (pipe->destAlphaPtr) {
      *pipe->destAlphaPtr++ = 255;
    }

  } else {


    switch (bitmap->mode) {
    case splashModeMono1:
      cDest[0] = (*pipe->destColorPtr & pipe->destColorMask) ? 0xff : 0x00;
      break;
    case splashModeMono8:
      cDest[0] = *pipe->destColorPtr;
      break;
    case splashModeRGB8:
      cDest[0] = pipe->destColorPtr[0];
      cDest[1] = pipe->destColorPtr[1];
      cDest[2] = pipe->destColorPtr[2];
      break;
    case splashModeXBGR8:
      cDest[0] = pipe->destColorPtr[2];
      cDest[1] = pipe->destColorPtr[1];
      cDest[2] = pipe->destColorPtr[0];
      cDest[3] = 255;
      break;
    case splashModeBGR8:
      cDest[0] = pipe->destColorPtr[2];
      cDest[1] = pipe->destColorPtr[1];
      cDest[2] = pipe->destColorPtr[0];
      break;
#if SPLASH_CMYK
    case splashModeCMYK8:
      cDest[0] = pipe->destColorPtr[0];
      cDest[1] = pipe->destColorPtr[1];
      cDest[2] = pipe->destColorPtr[2];
      cDest[3] = pipe->destColorPtr[3];
      break;
#endif
    }
    if (pipe->destAlphaPtr) {
      aDest = *pipe->destAlphaPtr;
    } else {
      aDest = 0xff;
    }


    if (state->blendFunc) {
      (*state->blendFunc)(pipe->cSrc, cDest, cBlend, bitmap->mode);
    }


    if (state->softMask) {
      if (pipe->usesShape) {
	aSrc = (Guchar)splashRound(pipe->aInput * *pipe->softMaskPtr++
				   * pipe->shape);
      } else {
	aSrc = (Guchar)splashRound(pipe->aInput * *pipe->softMaskPtr++);
      }
    } else if (pipe->usesShape) {
      aSrc = (Guchar)splashRound(pipe->aInput * pipe->shape);
    } else {
      aSrc = pipe->aSrc;
    }


    if (pipe->noTransparency) {
      alpha2 = aResult = 255;
    } else {
      aResult = aSrc + aDest - div255(aSrc * aDest);

      if (pipe->alpha0Ptr) {
	alpha0 = *pipe->alpha0Ptr++;
	alpha2 = aResult + alpha0 - div255(aResult * alpha0);
      } else {
	alpha2 = aResult;
      }
    }


    cResult0 = cResult1 = cResult2 = cResult3 = 0;  

    switch (pipe->resultColorCtrl) {

#if SPLASH_CMYK
    case splashPipeResultColorNoAlphaBlendCMYK:
      cResult3 = div255((255 - aDest) * pipe->cSrc[3] + aDest * cBlend[3]);
#endif
    case splashPipeResultColorNoAlphaBlendRGB:
      cResult2 = div255((255 - aDest) * pipe->cSrc[2] + aDest * cBlend[2]);
      cResult1 = div255((255 - aDest) * pipe->cSrc[1] + aDest * cBlend[1]);
    case splashPipeResultColorNoAlphaBlendMono:
      cResult0 = div255((255 - aDest) * pipe->cSrc[0] + aDest * cBlend[0]);
      break;

    case splashPipeResultColorAlphaNoBlendMono:
      if (alpha2 == 0) {
	cResult0 = 0;
      } else {
	cResult0 = (Guchar)(((alpha2 - aSrc) * cDest[0] +
			     aSrc * pipe->cSrc[0]) / alpha2);
      }
      break;
    case splashPipeResultColorAlphaNoBlendRGB:
      if (alpha2 == 0) {
	cResult0 = 0;
	cResult1 = 0;
	cResult2 = 0;
      } else {
	cResult0 = (Guchar)(((alpha2 - aSrc) * cDest[0] +
			     aSrc * pipe->cSrc[0]) / alpha2);
	cResult1 = (Guchar)(((alpha2 - aSrc) * cDest[1] +
			     aSrc * pipe->cSrc[1]) / alpha2);
	cResult2 = (Guchar)(((alpha2 - aSrc) * cDest[2] +
			     aSrc * pipe->cSrc[2]) / alpha2);
      }
      break;
#if SPLASH_CMYK
    case splashPipeResultColorAlphaNoBlendCMYK:
      if (alpha2 == 0) {
	cResult0 = 0;
	cResult1 = 0;
	cResult2 = 0;
	cResult3 = 0;
      } else {
	cResult0 = (Guchar)(((alpha2 - aSrc) * cDest[0] +
			     aSrc * pipe->cSrc[0]) / alpha2);
	cResult1 = (Guchar)(((alpha2 - aSrc) * cDest[1] +
			     aSrc * pipe->cSrc[1]) / alpha2);
	cResult2 = (Guchar)(((alpha2 - aSrc) * cDest[2] +
			     aSrc * pipe->cSrc[2]) / alpha2);
	cResult3 = (Guchar)(((alpha2 - aSrc) * cDest[3] +
			     aSrc * pipe->cSrc[3]) / alpha2);
      }
      break;
#endif

    case splashPipeResultColorAlphaBlendMono:
      if (alpha2 == 0) {
	cResult0 = 0;
      } else {
	cResult0 = (Guchar)(((alpha2 - aSrc) * cDest[0] +
			     aSrc * ((255 - aDest) * pipe->cSrc[0] +
				     aDest * cBlend[0]) / 255) /
			    alpha2);
      }
      break;
    case splashPipeResultColorAlphaBlendRGB:
      if (alpha2 == 0) {
	cResult0 = 0;
	cResult1 = 0;
	cResult2 = 0;
      } else {
	cResult0 = (Guchar)(((alpha2 - aSrc) * cDest[0] +
			     aSrc * ((255 - aDest) * pipe->cSrc[0] +
				     aDest * cBlend[0]) / 255) /
			    alpha2);
	cResult1 = (Guchar)(((alpha2 - aSrc) * cDest[1] +
			     aSrc * ((255 - aDest) * pipe->cSrc[1] +
				     aDest * cBlend[1]) / 255) /
			    alpha2);
	cResult2 = (Guchar)(((alpha2 - aSrc) * cDest[2] +
			     aSrc * ((255 - aDest) * pipe->cSrc[2] +
				     aDest * cBlend[2]) / 255) /
			    alpha2);
      }
      break;
#if SPLASH_CMYK
    case splashPipeResultColorAlphaBlendCMYK:
      if (alpha2 == 0) {
	cResult0 = 0;
	cResult1 = 0;
	cResult2 = 0;
	cResult3 = 0;
      } else {
	cResult0 = (Guchar)(((alpha2 - aSrc) * cDest[0] +
			     aSrc * ((255 - aDest) * pipe->cSrc[0] +
				     aDest * cBlend[0]) / 255) /
			    alpha2);
	cResult1 = (Guchar)(((alpha2 - aSrc) * cDest[1] +
			     aSrc * ((255 - aDest) * pipe->cSrc[1] +
				     aDest * cBlend[1]) / 255) /
			    alpha2);
	cResult2 = (Guchar)(((alpha2 - aSrc) * cDest[2] +
			     aSrc * ((255 - aDest) * pipe->cSrc[2] +
				     aDest * cBlend[2]) / 255) /
			    alpha2);
	cResult3 = (Guchar)(((alpha2 - aSrc) * cDest[3] +
			     aSrc * ((255 - aDest) * pipe->cSrc[3] +
				     aDest * cBlend[3]) / 255) /
			    alpha2);
      }
      break;
#endif
    }


    if (aResult != 0) {
      switch (pipe->nonIsolatedGroup) {
#if SPLASH_CMYK
      case 4:
	cResult3 += (cResult3 - cDest[3]) * aDest *
	            (255 - aResult) / (255 * aResult);
#endif
      case 3:
	cResult2 += (cResult2 - cDest[2]) * aDest *
	            (255 - aResult) / (255 * aResult);
	cResult1 += (cResult1 - cDest[1]) * aDest *
	            (255 - aResult) / (255 * aResult);
      case 1:
	cResult0 += (cResult0 - cDest[0]) * aDest *
	            (255 - aResult) / (255 * aResult);
      case 0:
	break;
      }
    }


    switch (bitmap->mode) {
    case splashModeMono1:
      if (state->screen->test(pipe->x, pipe->y, cResult0)) {
	*pipe->destColorPtr |= pipe->destColorMask;
      } else {
	*pipe->destColorPtr &= ~pipe->destColorMask;
      }
      if (!(pipe->destColorMask >>= 1)) {
	pipe->destColorMask = 0x80;
	++pipe->destColorPtr;
      }
      break;
    case splashModeMono8:
      *pipe->destColorPtr++ = cResult0;
      break;
    case splashModeRGB8:
      *pipe->destColorPtr++ = cResult0;
      *pipe->destColorPtr++ = cResult1;
      *pipe->destColorPtr++ = cResult2;
      break;
    case splashModeXBGR8:
      *pipe->destColorPtr++ = cResult2;
      *pipe->destColorPtr++ = cResult1;
      *pipe->destColorPtr++ = cResult0;
      *pipe->destColorPtr++ = 255;
      break;
    case splashModeBGR8:
      *pipe->destColorPtr++ = cResult2;
      *pipe->destColorPtr++ = cResult1;
      *pipe->destColorPtr++ = cResult0;
      break;
#if SPLASH_CMYK
    case splashModeCMYK8:
      *pipe->destColorPtr++ = cResult0;
      *pipe->destColorPtr++ = cResult1;
      *pipe->destColorPtr++ = cResult2;
      *pipe->destColorPtr++ = cResult3;
      break;
#endif
    }
    if (pipe->destAlphaPtr) {
      *pipe->destAlphaPtr++ = aResult;
    }

  }

  ++pipe->x;
}
