void RenderProcessHostImpl::PropagateBrowserCommandLineToRenderer(
    const base::CommandLine& browser_cmd,
    base::CommandLine* renderer_cmd) {
  static const char* const kSwitchNames[] = {
    switches::kAgcStartupMinVolume,
    switches::kAecRefinedAdaptiveFilter,
    switches::kAllowLoopbackInPeerConnection,
    switches::kAndroidFontsPath,
    switches::kAudioBufferSize,
    switches::kBlinkSettings,
    switches::kDefaultTileWidth,
    switches::kDefaultTileHeight,
    switches::kDisable2dCanvasImageChromium,
    switches::kDisable3DAPIs,
    switches::kDisableAcceleratedJpegDecoding,
    switches::kDisableAcceleratedVideoDecode,
    switches::kDisableBackgroundTimerThrottling,
    switches::kDisableBlinkFeatures,
    switches::kDisableBreakpad,
    switches::kDisablePreferCompositingToLCDText,
    switches::kDisableDatabases,
    switches::kDisableDisplayList2dCanvas,
    switches::kDisableDistanceFieldText,
    switches::kDisableFileSystem,
    switches::kDisableGestureRequirementForMediaPlayback,
    switches::kDisableGestureRequirementForPresentation,
    switches::kDisableGpuCompositing,
    switches::kDisableGpuMemoryBufferVideoFrames,
    switches::kDisableGpuVsync,
    switches::kDisableLowResTiling,
    switches::kDisableHistogramCustomizer,
    switches::kDisableLCDText,
    switches::kDisableLocalStorage,
    switches::kDisableLogging,
    switches::kDisableMediaSuspend,
    switches::kDisableNotifications,
    switches::kDisableOverlayScrollbar,
    switches::kDisablePepper3DImageChromium,
    switches::kDisablePermissionsAPI,
    switches::kDisablePresentationAPI,
    switches::kDisablePinch,
    switches::kDisableRGBA4444Textures,
    switches::kDisableRTCSmoothnessAlgorithm,
    switches::kDisableSeccompFilterSandbox,
    switches::kDisableSharedWorkers,
    switches::kDisableSlimmingPaintInvalidation,
    switches::kDisableSmoothScrolling,
    switches::kDisableSpeechAPI,
    switches::kDisableThreadedCompositing,
    switches::kDisableThreadedScrolling,
    switches::kDisableTouchAdjustment,
    switches::kDisableTouchDragDrop,
    switches::kDisableV8IdleTasks,
    switches::kDisableWebGLImageChromium,
    switches::kDomAutomationController,
    switches::kEnableBlinkFeatures,
    switches::kEnableBrowserSideNavigation,
    switches::kEnableColorCorrectRenderingDefaultMode,
    switches::kEnableDisplayList2dCanvas,
    switches::kEnableDistanceFieldText,
    switches::kEnableExperimentalCanvasFeatures,
    switches::kEnableExperimentalWebPlatformFeatures,
    switches::kEnableHeapProfiling,
    switches::kEnableGPUClientLogging,
    switches::kEnableGpuClientTracing,
    switches::kEnableGpuMemoryBufferVideoFrames,
    switches::kEnableGPUServiceLogging,
    switches::kEnableLowResTiling,
    switches::kEnableMediaSuspend,
    switches::kEnableInbandTextTracks,
    switches::kEnableLCDText,
    switches::kEnableLogging,
    switches::kEnableNetworkInformation,
    switches::kEnableOverlayScrollbar,
    switches::kEnablePinch,
    switches::kEnablePluginPlaceholderTesting,
    switches::kEnablePreciseMemoryInfo,
    switches::kEnablePrintBrowser,
    switches::kEnablePreferCompositingToLCDText,
    switches::kEnableRGBA4444Textures,
    switches::kEnableSkiaBenchmarking,
    switches::kEnableSlimmingPaintV2,
    switches::kEnableSlimmingPaintInvalidation,
    switches::kEnableSmoothScrolling,
    switches::kEnableStatsTable,
    switches::kEnableThreadedCompositing,
    switches::kEnableTouchDragDrop,
    switches::kEnableUseZoomForDSF,
    switches::kEnableViewport,
    switches::kEnableVp9InMp4,
    switches::kEnableVtune,
    switches::kEnableWebFontsInterventionTrigger,
    switches::kEnableWebFontsInterventionV2,
    switches::kEnableWebGLDraftExtensions,
    switches::kEnableWebGLImageChromium,
    switches::kEnableWebVR,
    switches::kExplicitlyAllowedPorts,
    switches::kForceDeviceScaleFactor,
    switches::kForceDisplayList2dCanvas,
    switches::kForceGpuMemAvailableMb,
    switches::kForceGpuRasterization,
    switches::kEnableCanvas2dDynamicRenderingModeSwitching,
    switches::kForceOverlayFullscreenVideo,
    switches::kFullMemoryCrashReport,
    switches::kInertVisualViewport,
    switches::kIPCConnectionTimeout,
    switches::kIsRunningInMash,
    switches::kJavaScriptFlags,
    switches::kLoggingLevel,
    switches::kMainFrameResizesAreOrientationChanges,
    switches::kMaxUntiledLayerWidth,
    switches::kMaxUntiledLayerHeight,
    switches::kMemoryMetrics,
    switches::kMojoLocalStorage,
    switches::kMSEAudioBufferSizeLimit,
    switches::kMSEVideoBufferSizeLimit,
    switches::kNoReferrers,
    switches::kNoSandbox,
    switches::kNoUseMusInRenderer,
    switches::kOverridePluginPowerSaverForTesting,
    switches::kPassiveListenersDefault,
    switches::kPpapiInProcess,
    switches::kProfilerTiming,
    switches::kReducedReferrerGranularity,
    switches::kReduceSecurityForTesting,
    switches::kRegisterPepperPlugins,
    switches::kRendererStartupDialog,
    switches::kRootLayerScrolls,
    switches::kShowPaintRects,
    switches::kSitePerProcess,
    switches::kStatsCollectionController,
    switches::kTestType,
    switches::kTopDocumentIsolation,
    switches::kTouchEventFeatureDetection,
    switches::kTouchTextSelectionStrategy,
    switches::kTraceConfigFile,
    switches::kTraceToConsole,
    switches::kUseFakeUIForMediaStream,
    switches::kUseGL,
    switches::kUseGpuInTests,
    switches::kUseMobileUserAgent,
    switches::kV,
    switches::kV8CacheStrategiesForCacheStorage,
    switches::kVideoThreads,
    switches::kVideoUnderflowThresholdMs,
    switches::kVModule,
    cc::switches::kCheckTilePriorityInversion,
    cc::switches::kDisableCompositedAntialiasing,
    cc::switches::kDisableThreadedAnimation,
    cc::switches::kEnableColorCorrectRendering,
    cc::switches::kEnableGpuBenchmarking,
    cc::switches::kEnableLayerLists,
    cc::switches::kEnableTileCompression,
    cc::switches::kEnableTrueColorRendering,
    cc::switches::kShowCompositedLayerBorders,
    cc::switches::kShowFPSCounter,
    cc::switches::kShowLayerAnimationBounds,
    cc::switches::kShowPropertyChangedRects,
    cc::switches::kShowScreenSpaceRects,
    cc::switches::kShowSurfaceDamageRects,
    cc::switches::kSlowDownRasterScaleFactor,
    cc::switches::kBrowserControlsHideThreshold,
    cc::switches::kBrowserControlsShowThreshold,

#if BUILDFLAG(ENABLE_PLUGINS)
    switches::kEnablePepperTesting,
#endif
#if BUILDFLAG(ENABLE_WEBRTC)
    switches::kDisableWebRtcHWDecoding,
    switches::kDisableWebRtcHWEncoding,
    switches::kEnableWebRtcStunOrigin,
    switches::kEnforceWebRtcIPPermissionCheck,
    switches::kForceWebRtcIPHandlingPolicy,
    switches::kWebRtcMaxCaptureFramerate,
#endif
    switches::kEnableLowEndDeviceMode,
    switches::kDisableLowEndDeviceMode,
#if defined(OS_ANDROID)
    switches::kDisableMediaSessionAPI,
    switches::kEnableContentIntentDetection,
    switches::kRendererWaitForJavaDebugger,
#endif
#if defined(OS_MACOSX)
    switches::kEnableSandboxLogging,
#endif
#if defined(OS_WIN)
    switches::kDisableWin32kLockDown,
    switches::kEnableWin7WebRtcHWH264Decoding,
    switches::kTrySupportedChannelLayouts,
    switches::kTraceExportEventsToETW,
#endif
#if defined(USE_OZONE)
    switches::kOzonePlatform,
#endif
#if defined(OS_CHROMEOS)
    switches::kDisableVaapiAcceleratedVideoEncode,
#endif
#if defined(ENABLE_IPC_FUZZER)
    switches::kIpcDumpDirectory,
    switches::kIpcFuzzerTestcase,
#endif
  };
  renderer_cmd->CopySwitchesFrom(browser_cmd, kSwitchNames,
                                 arraysize(kSwitchNames));

  BrowserChildProcessHostImpl::CopyFeatureAndFieldTrialFlags(renderer_cmd);

  if (browser_cmd.HasSwitch(switches::kTraceStartup) &&
      BrowserMainLoop::GetInstance()->is_tracing_startup_for_duration()) {
    renderer_cmd->AppendSwitchASCII(
        switches::kTraceStartup,
        browser_cmd.GetSwitchValueASCII(switches::kTraceStartup));
  }

#if BUILDFLAG(ENABLE_WEBRTC)
  if (!has_done_stun_trials &&
      browser_cmd.HasSwitch(switches::kWebRtcStunProbeTrialParameter)) {
    has_done_stun_trials = true;
    renderer_cmd->AppendSwitchASCII(
        switches::kWebRtcStunProbeTrialParameter,
        browser_cmd.GetSwitchValueASCII(
            switches::kWebRtcStunProbeTrialParameter));
  }
#endif

  if (GetBrowserContext()->IsOffTheRecord() &&
      !browser_cmd.HasSwitch(switches::kDisableDatabases)) {
    renderer_cmd->AppendSwitch(switches::kDisableDatabases);
  }

  if (browser_cmd.HasSwitch(switches::kWaitForDebuggerChildren)) {
    std::string value =
        browser_cmd.GetSwitchValueASCII(switches::kWaitForDebuggerChildren);
    if (value.empty() || value == switches::kRendererProcess) {
      renderer_cmd->AppendSwitch(switches::kWaitForDebugger);
    }
  }

  DCHECK(child_connection_);
  renderer_cmd->AppendSwitchASCII(switches::kPrimordialPipeToken,
                                  child_connection_->service_token());

#if defined(OS_WIN) && !defined(OFFICIAL_BUILD)
  if (renderer_cmd->HasSwitch(switches::kRendererStartupDialog) &&
      !renderer_cmd->HasSwitch(switches::kNoSandbox)) {
    renderer_cmd->AppendSwitch(switches::kNoSandbox);
  }
#endif
}
