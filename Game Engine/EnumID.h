#pragma once
#define _CRTDBG_MAP_ALLOC
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

namespace Gamestate
{
	enum ID
	{
		INITIALIZE, 
		LOAD_MENU,
		SHOW_MENU,
		CLEAR_MENU,
		LOAD_LEVEL,
		RUN_LEVEL,
		CLEAR_LEVEL,
		CLOSE_GAME
	};
	struct StateOfGame
	{
		static ID state;
	};
}

namespace Scene
{
	enum ID
	{
		EMPTY,
		MENU,
		LEVEL_1,
		LEVEL_2
	};
}

namespace ComponentType
{
	enum ID
	{
		PLAYER,
		COMPONENT,
		TERRAIN,
		LIGHT,
		MATERIAL,
		MAINMENU,
		MESHFILTER,
		AI
	};
}

namespace ObjectType
{
	enum ID
	{
		Player						= 0,
		Bucket_Empty				= 1,
		Stone_1						= 2,
		Campfire					= 3,
		Cliffside_1					= 4,
		Cliffside_2					= 5,
		Cliffside_3					= 6,
		Cliffside_4					= 7,
		Campfire_NoSnow				= 8,
		WoodenSign					= 9,
		Pine_Tree					= 10,
		Pine_Tree_Snow				= 11,
		BrokenTree					= 12,
		BrokenTree_Snow				= 13,
		IceTaps_1					= 14,
		IceTaps_2					= 15,
		WaterBottle					= 16,
		Flag						= 17,
		Can							= 18,
		Bucket_Water				= 19,
		Bucket_Snow					= 20,
		Stump						= 21,
		Tree						= 22,
		TreeWithSnow				= 23,
		FlareGunBox					= 24,
		Axe							= 25,
		Jacket						= 26,
		Stone_2						= 27,
		Stone_3						= 28,
		Stone_4						= 29,
		Stone_5						= 30,
		Cabin						= 31,
		DeadTree					= 32,
		Equiped_Axe					= 33,			
		EquipedEmptyBucket			= 34,
		EquipedEmptyBucketSnow		= 35,
		EquipedEmptyBucketWater		= 36,
		Tree_Small					= 37,
		DeadTree_Small				= 38,
		DeadTreeSnow				= 39,
		DeadTreeSnow_Small			= 40,
		Tree_Small_Snow				= 41,
		FlareGun					= 42,
		IceWall						= 43,
		EquipedLighter				= 44,
		EquipedWood					= 45,
		EquipedCan					= 46,
		EquipedBucketEmptyWarm		= 47,
		EquipedBucketSnowWarm		= 48,
		EquipedBucketWaterWarm		= 49,
		EquipedAxeWard				= 50,
		EquipedCanWard				= 51,
		EquipedLighterWarm			= 52,
		EquipedWoodWarm				= 53,
		Rabbit						= 54,
		Cabin_rotate				= 55,
		BrokenTree_rotate			= 56,
		Balloon						= 57,
		MatchBox					= 58,
		Wooden_Step					= 59,
		Backpack					= 60
	};
}