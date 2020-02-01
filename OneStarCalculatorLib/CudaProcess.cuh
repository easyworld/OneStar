#pragma once
#include "Type.h"

struct CudaInputMaster
{
	int ivs[6];
	_u32 constantTermVector[2];
	_u32 answerFlag[128];
};

// 結果
extern _u32* cu_HostResult;

void CudaInitialize(int* pIvs);
void CudaProcess(_u32 ivs, int freeBit); //処理関数
void CudaFinalize();
