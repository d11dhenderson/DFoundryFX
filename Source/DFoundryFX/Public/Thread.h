#pragma once
// Thread
#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "HAL/ThreadSafeBool.h"
#include "Misc/SingleThreadRunnable.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Async/Async.h"

// Events -> GameMode and Viewport
#include "UnrealClient.h"
#include "Engine/GameViewportClient.h"
#include "Engine/GameViewportDelegates.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/HUD.h"
#include "Engine/GameInstance.h"

// ImGui
#include "Engine/Engine.h"
#include "Engine/Canvas.h"
#include "GameFramework/PlayerController.h"
#include "GenericPlatform/GenericPlatformApplicationMisc.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "CanvasTypes.h"
#include "imgui.h"

// Stats
#include "StatData.h"

class DFOUNDRYFX_API FDFX_Thread : public FRunnable, FSingleThreadRunnable
{
public:  //Thread
  FDFX_Thread();
  virtual ~FDFX_Thread();

  void Wait(float Seconds);
  virtual FSingleThreadRunnable* GetSingleThreadInterface() override { return this; }
  virtual void Tick() override;
  virtual void DFXTick();

  virtual bool Init() override;
  virtual uint32 Run() override;
  virtual void Stop() override;
  virtual void Exit() override;

  void SetPaused(bool MakePaused);
  bool IsThreadPaused();
  bool IsThreadVerifiedSuspended();
  bool HasThreadStopped();

protected:
  bool bStopping = false;
  FThreadSafeBool bPaused;
  FThreadSafeBool bIsVerifiedSuspended;
  FThreadSafeBool bHasStopped;
  FRunnableThread* DFoundryFX_Thread = nullptr;


public:  // Events -> GameMode and Viewport
  AGameModeBase* GameMode;
  UGameViewportClient* GameViewport;
  FVector2D ViewportSize;
  UWorld* uWorld;
  UCanvas* uCanvas;
  APlayerController* PlayerController;

  FDelegateHandle hOnGameModeInitialized;
  void OnGameModeInitialized(AGameModeBase* GameMode);

  FDelegateHandle hOnWorldBeginPlay;
  void OnWorldBeginPlay();

  FDelegateHandle hOnViewportCreated;
  void OnViewportCreated();

  FDelegateHandle hOnHUDPostRender;
  void OnHUDPostRender(AHUD* Sender, UCanvas* Canvas);

  FDelegateHandle hOnViewportClose;
  bool OnViewportClose();


public:  // ImGui
  bool ImGui_ImplUE_Init();
  bool ImGui_ImplUE_CreateDeviceObjects();
  void ImGui_ImplUE_ProcessEvent();
  void ImGui_ImplUE_NewFrame();
  void ImGui_ImplUE_Render();
  void ImGui_ImplUE_RenderDrawLists();

  static const char* ImGui_ImplUE_GetClipboardText(void* user_data);
  static void ImGui_ImplUE_SetClipboardText(void* user_data, const char* text);

  UMaterialInterface* MasterMaterial = nullptr;
  UMaterialInstanceDynamic* MaterialInstance = nullptr;
  UTexture2D* FontTexture = nullptr;


private:
  FORCEINLINE ImGuiIO& GetImGuiIO() const;
  ImGuiContext* m_ImGuiContext = nullptr;
  ImPlotContext* m_ImPlotContext = nullptr;

  static inline uint64 m_ImGuiDiffTime;

  static ImGuiKey FKeyToImGuiKey(FName Keyname);
};