#include "Data.h"
#include "Util.h"

// �v�Z�p�f�[�^
_u64 g_InputMatrix[64]; // CalculateInverseMatrix�̑O�ɃZ�b�g����
_u64 g_ConstantTermVector;
_u64 g_Coefficient[64];
_u64 g_AnswerFlag[64];
int g_FreeBit[64];
int g_FreeId[64];

_u64 g_CoefficientData[0x4000];
_u64 g_SearchPattern[0x4000];

void CalculateInverseMatrix(int length)
{
	// ������Ԃ��Z�b�g
	for (int i = 0; i < length; ++i)
	{
		g_AnswerFlag[i] = (1ull << (length - 1 - i));
	}

	int skip = 0;
	for (int i = 0; i < 64; ++i)
	{
		g_FreeBit[i] = 0;
	}

	// �s��{�ό`�ŋ��߂�
	for (int rank = 0; rank < length; )
	{
		_u64 top = (1ull << (63 - (rank + skip)));
		bool rankUpFlag = false;
		for (int i = rank; i < length; ++i)
		{
			if ((g_InputMatrix[i] & top) != 0) // ��ԍ���1
			{
				for (int a = 0; a < length; ++a)
				{
					if (a == i) continue;

					// �������1������
					if ((g_InputMatrix[a] & top) != 0)
					{
						g_InputMatrix[a] ^= g_InputMatrix[i];
						g_AnswerFlag[a] ^= g_AnswerFlag[i];
					}
				}
				// �Ō�Ɉ�ԏ�Ɏ����Ă���
				_u64 swapM = g_InputMatrix[rank];
				_u64 swapF = g_AnswerFlag[rank];
				g_InputMatrix[rank] = g_InputMatrix[i];
				g_AnswerFlag[rank] = g_AnswerFlag[i];
				g_InputMatrix[i] = swapM;
				g_AnswerFlag[i] = swapF;

				++rank;
				rankUpFlag = true;
			}
		}
		if (rankUpFlag == false)
		{
			// �}�[�L���O���ăX�L�b�v
			g_FreeBit[rank + skip] = 1;
			g_FreeId[skip] = rank + skip;
			++skip;
		}
	}
	// ���Rbit
	for (int i = length + skip; i < 64; ++i)
	{
		g_FreeBit[i] = 1;
		g_FreeId[i - length] = i;
	}

	// �W���������������o��
	for (int i = 0; i < length; ++i)
	{
		g_Coefficient[i] = 0;
		for (int a = 0; a < (64 - length); ++a)
		{
			g_Coefficient[i] |= (g_InputMatrix[i] & (1ull << (63 - g_FreeId[a]))) >> ((length + a) - g_FreeId[a]);
		}
	}
}

void CalculateCoefficientData(int length)
{
	// �f�[�^�����
	unsigned short max = ((1 << (64 - length)) - 1);
	for (unsigned short search = 0; search <= max; ++search)
	{
		g_CoefficientData[search] = 0;
		g_SearchPattern[search] = 0;
		int offset = 0;
		for (int i = 0; i < length; ++i)
		{
			while (g_FreeBit[i + offset] > 0)
			{
				++offset;
			}
			g_CoefficientData[search] |= (GetSignature(g_Coefficient[i] & search) << (63 - (i + offset)));
		}
		for (int a = 0; a < (64 - length); ++a)
		{
			g_SearchPattern[search] |= ((_u64)search & (1ull << (64 - length - 1 - a))) << ((length + a) - g_FreeId[a]);
		}
	}
}