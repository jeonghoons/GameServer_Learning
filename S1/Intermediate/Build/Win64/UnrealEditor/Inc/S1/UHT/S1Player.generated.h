// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Game/S1Player.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef S1_S1Player_generated_h
#error "S1Player.generated.h already included, missing '#pragma once' in S1Player.h"
#endif
#define S1_S1Player_generated_h

#define FID_GameServer_Study_S1_Source_S1_Game_S1Player_h_23_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAS1Player(); \
	friend struct Z_Construct_UClass_AS1Player_Statics; \
public: \
	DECLARE_CLASS(AS1Player, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/S1"), NO_API) \
	DECLARE_SERIALIZER(AS1Player)


#define FID_GameServer_Study_S1_Source_S1_Game_S1Player_h_23_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	AS1Player(AS1Player&&); \
	AS1Player(const AS1Player&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AS1Player); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AS1Player); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AS1Player)


#define FID_GameServer_Study_S1_Source_S1_Game_S1Player_h_20_PROLOG
#define FID_GameServer_Study_S1_Source_S1_Game_S1Player_h_23_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_GameServer_Study_S1_Source_S1_Game_S1Player_h_23_INCLASS_NO_PURE_DECLS \
	FID_GameServer_Study_S1_Source_S1_Game_S1Player_h_23_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> S1_API UClass* StaticClass<class AS1Player>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_GameServer_Study_S1_Source_S1_Game_S1Player_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
