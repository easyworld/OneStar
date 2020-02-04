#pragma once
#include "Type.h"
#include "Util.h"

struct CudaInputMaster
{
	// seed計算定数
	_u32 constantTermVector[2];
	_u32 answerFlag[128];
	_u32 coefficientData[0x8000];
	_u32 searchPattern[0x4000];

	// 検索条件
	int ecBit;
	bool ecMod[3][6];
	int ivs[6];
	PokemonData pokemon[4];
};

// 入力
extern CudaInputMaster* cu_HostMaster;

// 結果
extern int* cu_HostResultCount;
extern _u64* cu_HostResult;

void CudaInitializeImpl();
void CudaSetMasterData(int length);

void CudaProcess(_u32 param, int partition); //処理関数
void CudaFinalize();
