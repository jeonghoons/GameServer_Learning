// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Game/S1MyPlayer.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef S1_S1MyPlayer_generated_h
#error "S1MyPlayer.generated.h already included, missing '#pragma once' in S1MyPlayer.h"
#endif
#define S1_S1MyPlayer_generated_h

#define FID_GameServer_Study_S1_Source_S1_Game_S1MyPlayer_h_21_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAS1MyPlayer(); \
	friend struct Z_Construct_UClass_AS1MyPlayer_Statics; \
public: \
	DECLARE_CLASS(AS1MyPlayer, AS1Player, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/S1"), NO_API) \
	DECLARE_SERIALIZER(AS1MyPlayer)


#define FID_GameServer_Study_S1_Source_S1_Game_S1MyPlayer_h_21_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	AS1MyPlayer(AS1MyPlayer&&); \
	AS1MyPlayer(const AS1MyPlayer&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AS1MyPlayer); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AS1MyPlayer); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AS1MyPlayer) \
	NO_API virtual ~AS1MyPlayer();


#define FID_GameServer_Study_S1_Source_S1_Game_S1MyPlayer_h_18_PROLOG
#define FID_GameServer_Study_S1_Source_S1_Game_S1MyPlayer_h_21_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_GameServer_Study_S1_Source_S1_Game_S1MyPlayer_h_21_INCLASS_NO_PURE_DECLS \
	FID_GameServer_Study_S1_Source_S1_Game_S1MyPlayer_h_21_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> S1_API UClass* StaticClass<class AS1MyPlayer>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_GameServer_Study_S1_Source_S1_Game_S1MyPlayer_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
