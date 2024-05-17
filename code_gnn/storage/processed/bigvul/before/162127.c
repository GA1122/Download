void RenderProcessHostImpl::PropagateBrowserCommandLineToRenderer(
    const base::CommandLine& browser_cmd,
    base::CommandLine* renderer_cmd) {
  static const char* const kSwitchNames[] = {
    service_manager::switches::kDisableInProcessStackTraces,
    service_manager::switches::kDisableSeccompFilterSandbox,
    switches::kAgcStartupMinVolume,
    switches::kAecRefinedAdaptiveFilter,
    switches::kAllowLoopbackInPeerConnection,
    switches::kAndroidFontsPath,
    switches::kAudioBufferSize,
    switches::kAutoplayPolicy,
    switches::kBlinkSettings,
    switches::kDefaultTileWidth,
    switches::kDefaultTileHeight,
    switches::kDisable2dCanvasImageChromium,
    switches::kDisableAcceleratedJpegDecoding,
    switches::kDisableAcceleratedVideoDecode,
    switches::kDisableBackgroundTimerThrottling,
    switches::kDisableBreakpad,
    switches::kDisableCompositorUkmForTests,
    switches::kDisablePreferCompositingToLCDText,
    switches::kDisableDatabases,
    switches::kDisableDistanceFieldText,
    switches::kDisableFileSystem,
    switches::kDisableGpuMemoryBufferVideoFrames,
    switches::kDisableGpuVsync,
    switches::kDisableLowResTiling,
    switches::kDisableHistogramCustomizer,
    switches::kDisableLCDText,
    switches::kDisableLogging,
    switches::kDisableMediaSuspend,
    switches::kDisableNotifications,
    switches::kDisableOriginTrialControlledBlinkFeatures,
    switches::kDisablePepper3DImageChromium,
    switches::kDisablePermissionsAPI,
    switches::kDisablePresentationAPI,
    switches::kDisableRGBA4444Textures,
    switches::kDisableRTCSmoothnessAlgorithm,
    switches::kDisableSharedWorkers,
    switches::kDisableSkiaRuntimeOpts,
    switches::kDisableSpeechAPI,
    switches::kDisableThreadedCompositing,
    switches::kDisableThreadedScrolling,
    switches::kDisableTouchAdjustment,
    switches::kDisableTouchDragDrop,
    switches::kDisableV8IdleTasks,
    switches::kDisableWebGLImageChromium,
    switches::kDomAutomationController,
    switches::kEnableAutomation,
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
    switches::kEnableNetworkInformationDownlinkMax,
    switches::kEnableOOPRasterization,
    switches::kEnablePluginPlaceholderTesting,
    switches::kEnablePreciseMemoryInfo,
    switches::kEnablePrintBrowser,
    switches::kEnablePreferCompositingToLCDText,
    switches::kEnableRGBA4444Textures,
    switches::kEnableSkiaBenchmarking,
    switches::kEnableSlimmingPaintV175,
    switches::kEnableSlimmingPaintV2,
    switches::kEnableThreadedCompositing,
    switches::kEnableTouchDragDrop,
    switches::kEnableUseZoomForDSF,
    switches::kEnableViewport,
    switches::kEnableVtune,
    switches::kEnableWebGLDraftExtensions,
    switches::kEnableWebGLImageChromium,
    switches::kEnableWebVR,
    switches::kExplicitlyAllowedPorts,
    switches::kForceColorProfile,
    switches::kForceDeviceScaleFactor,
    switches::kForceGpuMemAvailableMb,
    switches::kForceGpuRasterization,
    switches::kForceOverlayFullscreenVideo,
    switches::kForceVideoOverlays,
    switches::kFullMemoryCrashReport,
    switches::kIPCConnectionTimeout,
    switches::kJavaScriptFlags,
    switches::kLoggingLevel,
    switches::kMaxUntiledLayerWidth,
    switches::kMaxUntiledLayerHeight,
    switches::kDisableMojoLocalStorage,
    switches::kMSEAudioBufferSizeLimit,
    switches::kMSEVideoBufferSizeLimit,
    switches::kNoReferrers,
    switches::kNoSandbox,
    switches::kNoZygote,
    switches::kOverridePluginPowerSaverForTesting,
    switches::kPassiveListenersDefault,
    switches::kPpapiInProcess,
    switches::kReducedReferrerGranularity,
    switches::kRegisterPepperPlugins,
    switches::kRendererStartupDialog,
    switches::kRootLayerScrolls,
    switches::kShowPaintRects,
    switches::kStatsCollectionController,
    switches::kTestType,
    switches::kTouchEventFeatureDetection,
    switches::kTouchTextSelectionStrategy,
    switches::kTraceConfigFile,
    switches::kTraceToConsole,
    switches::kUseFakeUIForMediaStream,
    switches::kUseGL,
    switches::kUseGpuInTests,
    switches::kUseMobileUserAgent,
    switches::kV,
    switches::kVideoThreads,
    switches::kVideoUnderflowThresholdMs,
    switches::kVModule,
    cc::switches::kDisableCompositedAntialiasing,
    cc::switches::kDisableThreadedAnimation,
    cc::switches::kEnableGpuBenchmarking,
    cc::switches::kEnableLayerLists,
    cc::switches::kEnableTileCompression,
    cc::switches::kShowCompositedLayerBorders,
    cc::switches::kShowFPSCounter,
    cc::switches::kShowLayerAnimationBounds,
    cc::switches::kShowPropertyChangedRects,
    cc::switches::kShowScreenSpaceRects,
    cc::switches::kShowSurfaceDamageRects,
    cc::switches::kSlowDownRasterScaleFactor,
    cc::switches::kBrowserControlsHideThreshold,
    cc::switches::kBrowserControlsShowThreshold,
    cc::switches::kRunAllCompositorStagesBeforeDraw,
    switches::kDisableSurfaceReferences,
    switches::kEnableSurfaceSynchronization,

#if BUILDFLAG(ENABLE_PLUGINS)
    switches::kEnablePepperTesting,
#endif
#if BUILDFLAG(ENABLE_RUNTIME_MEDIA_RENDERER_SELECTION)
    switches::kDisableMojoRenderer,
#endif
#if BUILDFLAG(ENABLE_WEBRTC)
    switches::kDisableWebRtcHWDecoding,
    switches::kDisableWebRtcHWEncoding,
    switches::kEnableWebRtcSrtpAesGcm,
    switches::kEnableWebRtcSrtpEncryptedHeaders,
    switches::kEnableWebRtcStunOrigin,
    switches::kEnforceWebRtcIPPermissionCheck,
    switches::kWebRtcMaxCaptureFramerate,
#endif
    switches::kEnableLowEndDeviceMode,
    switches::kDisableLowEndDeviceMode,
    switches::kDisallowNonExactResourceReuse,
#if defined(OS_ANDROID)
    switches::kDisableMediaSessionAPI,
    switches::kMadviseRandomExecutableCode,
    switches::kRendererWaitForJavaDebugger,
#endif
#if defined(OS_MACOSX)
    switches::kEnableSandboxLogging,
#endif
#if defined(OS_WIN)
    service_manager::switches::kDisableWin32kLockDown,
    switches::kEnableWin7WebRtcHWH264Decoding,
    switches::kTrySupportedChannelLayouts,
    switches::kTraceExportEventsToETW,
#endif
#if defined(USE_OZONE)
    switches::kOzonePlatform,
#endif
#if defined(ENABLE_IPC_FUZZER)
    switches::kIpcDumpDirectory,
    switches::kIpcFuzzerTestcase,
#endif
#if BUILDFLAG(ENABLE_MUS)
    switches::kMus,
    switches::kMusHostingViz,
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

#if !defined(OS_ANDROID) && !defined(OS_CHROMEOS)
#if !BUILDFLAG(ENABLE_MUS)
  if (ImageTransportFactory::GetInstance()->IsGpuCompositingDisabled())
    renderer_cmd->AppendSwitch(switches::kDisableGpuCompositing);
#else
#endif
#endif

  if (browser_cmd.HasSwitch(switches::kWaitForDebuggerChildren)) {
    std::string value =
        browser_cmd.GetSwitchValueASCII(switches::kWaitForDebuggerChildren);
    if (value.empty() || value == switches::kRendererProcess) {
      renderer_cmd->AppendSwitch(switches::kWaitForDebugger);
    }
  }

  DCHECK(child_connection_);
  renderer_cmd->AppendSwitchASCII(service_manager::switches::kServicePipeToken,
                                  child_connection_->service_token());

#if defined(OS_WIN) && !defined(OFFICIAL_BUILD)
  if (renderer_cmd->HasSwitch(switches::kRendererStartupDialog) &&
      !renderer_cmd->HasSwitch(switches::kNoSandbox)) {
    renderer_cmd->AppendSwitch(switches::kNoSandbox);
  }
#endif

  CopyFeatureSwitch(browser_cmd, renderer_cmd, switches::kEnableBlinkFeatures);
  CopyFeatureSwitch(browser_cmd, renderer_cmd, switches::kDisableBlinkFeatures);
}
