// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "S1/Game/S1Player.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeS1Player() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_ACharacter();
S1_API UClass* Z_Construct_UClass_AS1Player();
S1_API UClass* Z_Construct_UClass_AS1Player_NoRegister();
UPackage* Z_Construct_UPackage__Script_S1();
// End Cross Module References

// Begin Class AS1Player
void AS1Player::StaticRegisterNativesAS1Player()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AS1Player);
UClass* Z_Construct_UClass_AS1Player_NoRegister()
{
	return AS1Player::StaticClass();
}
struct Z_Construct_UClass_AS1Player_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "Game/S1Player.h" },
		{ "ModuleRelativePath", "Game/S1Player.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AS1Player>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_AS1Player_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ACharacter,
	(UObject* (*)())Z_Construct_UPackage__Script_S1,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AS1Player_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AS1Player_Statics::ClassParams = {
	&AS1Player::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AS1Player_Statics::Class_MetaDataParams), Z_Construct_UClass_AS1Player_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AS1Player()
{
	if (!Z_Registration_Info_UClass_AS1Player.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AS1Player.OuterSingleton, Z_Construct_UClass_AS1Player_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AS1Player.OuterSingleton;
}
template<> S1_API UClass* StaticClass<AS1Player>()
{
	return AS1Player::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AS1Player);
// End Class AS1Player

// Begin Registration
struct Z_CompiledInDeferFile_FID_GameServer_Study_S1_Source_S1_Game_S1Player_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AS1Player, AS1Player::StaticClass, TEXT("AS1Player"), &Z_Registration_Info_UClass_AS1Player, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AS1Player), 1207210634U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_GameServer_Study_S1_Source_S1_Game_S1Player_h_886166494(TEXT("/Script/S1"),
	Z_CompiledInDeferFile_FID_GameServer_Study_S1_Source_S1_Game_S1Player_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_GameServer_Study_S1_Source_S1_Game_S1Player_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
