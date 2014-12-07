// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2013 - 2014, LH_Mouse. All wrongs reserved.

#include "avl_tree.h"
#include "../ext/assert.h"

static inline size_t GetHeight(const MCF_AvlNodeHeader *pWhere){
	return pWhere ? pWhere->uHeight : 0;
}
static inline size_t Max(size_t lhs, size_t rhs){
	return (lhs > rhs) ? lhs : rhs;
}
static void UpdateRecur(MCF_AvlNodeHeader *pWhere){
	ASSERT(pWhere);

	MCF_AvlNodeHeader *pNode = pWhere;
	size_t uLeftHeight = GetHeight(pWhere->pLeft);
	size_t uRightHeight = GetHeight(pWhere->pRight);
	for(;;){
		const size_t uOldHeight = pNode->uHeight;

		MCF_AvlNodeHeader *const pParent = pNode->pParent;
		MCF_AvlNodeHeader **const ppRefl = pNode->ppRefl;
		MCF_AvlNodeHeader *const pLeft = pNode->pLeft;
		MCF_AvlNodeHeader *const pRight = pNode->pRight;

		if(uLeftHeight > uRightHeight){
			ASSERT(uLeftHeight - uRightHeight <= 2);

			if(uLeftHeight - uRightHeight <= 1){
				pNode->uHeight = uLeftHeight + 1;
			} else {
				ASSERT(pLeft);

				MCF_AvlNodeHeader *const pLL = pLeft->pLeft;
				MCF_AvlNodeHeader *const pLR = pLeft->pRight;

				const size_t uLLHeight = GetHeight(pLL);
				const size_t uLRHeight = GetHeight(pLR);
				if(uLLHeight >= uLRHeight){
					ASSERT(pLL);

					/*------------------+-------------------*\
					|        node       |    left            |
					|       /    \      |   /    \           |
					|   left      right > ll      node       |
					|  /    \           |        /    \      |
					| ll     lr         |      lr      right |
					\*------------------+-------------------*/

					if(pLR){
						pLR->pParent = pNode;
						pLR->ppRefl = &(pNode->pLeft);
					}

					pLL->pParent = pLeft;
					pLL->ppRefl = &(pLeft->pLeft);

					pNode->pParent = pLeft;
					pNode->ppRefl = &(pLeft->pRight);
					pNode->pLeft = pLR;
					// H(lr) >= H(l) - 2   // 平衡二叉树的要求。
					//        = H(r)       // 前置条件。
					pNode->uHeight = uLRHeight + 1;

					pLeft->pParent = pParent;
					pLeft->ppRefl = ppRefl;
					pLeft->pRight = pNode;
					pLeft->uHeight = Max(pLL->uHeight, pNode->uHeight) + 1;

					*ppRefl = pLeft;
					pNode = pLeft;
				} else {
					ASSERT(pLR);

					/*-------------------+----------------------------*\
					|         node       |         __ lr __            |
					|        /    \      |        /        \           |
					|    left      right |    left          node       |
					|   /    \           >   /    \        /    \      |
					| ll      lr         | ll      lrl  lrr      right |
					|        /  \        |                             |
					|     lrl    lrr     |                             |
					\*-------------------+----------------------------*/

					MCF_AvlNodeHeader *const pLRL = pLR->pLeft;
					MCF_AvlNodeHeader *const pLRR = pLR->pRight;

					size_t uLRLHeight = 0;
					if(pLRL){
						pLRL->pParent = pLeft;
						pLRL->ppRefl = &(pLeft->pRight);
						uLRLHeight = pLRL->uHeight;
					}

					size_t uLRRHeight = 0;
					if(pLRR){
						pLRR->pParent = pNode;
						pLRR->ppRefl = &(pNode->pLeft);
						uLRRHeight = pLRR->uHeight;
					}

					pLeft->pParent = pLR;
					pLeft->ppRefl = &(pLR->pLeft);
					pLeft->pRight = pLRL;
					pLeft->uHeight = Max(GetHeight(pLeft->pLeft), uLRLHeight) + 1;

					pNode->pParent = pLR;
					pNode->ppRefl = &(pLR->pRight);
					pNode->pLeft = pLRR;
					pNode->uHeight = Max(uLRRHeight, GetHeight(pNode->pRight)) + 1;

					pLR->pParent = pParent;
					pLR->ppRefl = ppRefl;
					pLR->pLeft = pLeft;
					pLR->pRight = pNode;
					pLR->uHeight = Max(pLeft->uHeight, pNode->uHeight) + 1;

					*ppRefl = pLR;
					pNode = pLR;
				}
			}
		} else {
			ASSERT(uRightHeight - uLeftHeight <= 2);

			if(uRightHeight - uLeftHeight <= 1){
				pNode->uHeight = uRightHeight + 1;
			} else {
				ASSERT(pRight);
				ASSERT(GetHeight(pRight) - GetHeight(pNode->pLeft) == 2);

				MCF_AvlNodeHeader *const pRR = pRight->pRight;
				MCF_AvlNodeHeader *const pRL = pRight->pLeft;

				const size_t uRRHeight = GetHeight(pRR);
				const size_t uRLHeight = GetHeight(pRL);
				if(uRRHeight >= uRLHeight){
					ASSERT(pRR);

					if(pRL){
						pRL->pParent = pNode;
						pRL->ppRefl = &(pNode->pRight);
					}

					pRR->pParent = pRight;
					pRR->ppRefl = &(pRight->pRight);

					pNode->pParent = pRight;
					pNode->ppRefl = &(pRight->pLeft);
					pNode->pRight = pRL;
					pNode->uHeight = uRLHeight + 1;

					pRight->pParent = pParent;
					pRight->ppRefl = ppRefl;
					pRight->pLeft = pNode;
					pRight->uHeight = Max(pRR->uHeight, pNode->uHeight) + 1;

					*ppRefl = pRight;
					pNode = pRight;
				} else {
					ASSERT(pRL);

					MCF_AvlNodeHeader *const pRLR = pRL->pRight;
					MCF_AvlNodeHeader *const pRLL = pRL->pLeft;

					size_t uRLRHeight = 0;
					if(pRLR){
						pRLR->pParent = pRight;
						pRLR->ppRefl = &(pRight->pLeft);
						uRLRHeight = pRLR->uHeight;
					}

					size_t uRLLHeight = 0;
					if(pRLL){
						pRLL->pParent = pNode;
						pRLL->ppRefl = &(pNode->pRight);
						uRLLHeight = pRLL->uHeight;
					}

					pRight->pParent = pRL;
					pRight->ppRefl = &(pRL->pRight);
					pRight->pLeft = pRLR;
					pRight->uHeight = Max(GetHeight(pRight->pRight), uRLRHeight) + 1;

					pNode->pParent = pRL;
					pNode->ppRefl = &(pRL->pLeft);
					pNode->pRight = pRLL;
					pNode->uHeight = Max(uRLLHeight, GetHeight(pNode->pLeft)) + 1;

					pRL->pParent = pParent;
					pRL->ppRefl = ppRefl;
					pRL->pRight = pRight;
					pRL->pLeft = pNode;
					pRL->uHeight = Max(pRight->uHeight, pNode->uHeight) + 1;

					*ppRefl = pRL;
					pNode = pRL;
				}
			}
		}
		if(!pParent){
			break;
		}

		const size_t uNewHeight = pNode->uHeight;
		if(uOldHeight == uNewHeight){
			break;
		}

		if(ppRefl == &(pParent->pLeft)){
			uLeftHeight = uNewHeight;
			uRightHeight = GetHeight(pParent->pRight);
		} else {
			uLeftHeight = GetHeight(pParent->pLeft);
			uRightHeight = uNewHeight;
		}
		pNode = pParent;
	}
}

void MCF_AvlInternalAttach(MCF_AvlNodeHeader *pNode,
	MCF_AvlNodeHeader *pParent, MCF_AvlNodeHeader **ppRefl)
{
	ASSERT(*ppRefl == nullptr);

	*ppRefl = pNode;

	pNode->pParent	= pParent;
	pNode->ppRefl	= ppRefl;
	pNode->pLeft	= nullptr;
	pNode->pRight	= nullptr;
	pNode->uHeight	= 1;

	if(!pParent){
		pNode->pPrev = nullptr;
		pNode->pNext = nullptr;
	} else {
		if(ppRefl == &(pParent->pLeft)){
			MCF_AvlNodeHeader *const pPrev = pParent->pPrev;
			pNode->pPrev = pPrev;
			pNode->pNext = pParent;
			pParent->pPrev = pNode;
			if(pPrev){
				pPrev->pNext = pNode;
			}
		} else {
			MCF_AvlNodeHeader *const pNext = pParent->pNext;
			pNode->pPrev = pParent;
			pNode->pNext = pNext;
			if(pNext){
				pNext->pPrev = pNode;
			}
			pParent->pNext = pNode;
		}
		if(pParent){
			UpdateRecur(pParent);
		}
	}
}

void MCF_AvlInternalDetach(const MCF_AvlNodeHeader *pNode){
	MCF_AvlNodeHeader *const pParent = pNode->pParent;
	MCF_AvlNodeHeader **const ppRefl = pNode->ppRefl;
	MCF_AvlNodeHeader *const pLeft = pNode->pLeft;
	MCF_AvlNodeHeader *const pRight = pNode->pRight;
	MCF_AvlNodeHeader *const pPrev = pNode->pPrev;
	MCF_AvlNodeHeader *const pNext = pNode->pNext;

	if(!pLeft){
		/*-----------+------*\
		| node       | right |
		|     \      >       |
		|      right |       |
		\*-----------+------*/

		*ppRefl = pRight;

		if(pRight){
			pRight->pParent = pParent;
			pRight->ppRefl = ppRefl;
		}

		if(pParent){
			UpdateRecur(pParent);
		}
	} else {
		if(pPrev == pLeft){
			/*------------------+--------------*\
			|        node       |    left       |
			|       /    \      |   /    \      |
			|   left      right > ll      right |
			|  /                |               |
			| ll                |               |
			\*------------------+--------------*/

			*ppRefl = pLeft;

			if(pRight){
				pRight->pParent = pLeft;
				pRight->ppRefl = &(pLeft->pRight);
			}

			pLeft->pParent	= pParent;
			pLeft->ppRefl	= ppRefl;
			pLeft->pRight	= pRight;
			pLeft->uHeight	= pNode->uHeight;

			UpdateRecur(pLeft);
		} else {
			/*--------------------------+--------------------------*\
			|      ____ node ____       |      ____ prev ____       |
			|     /              \      |     /              \      |
			| left                right | left                right |
			|     \                     |     \                     |
			|      prevp                |      prevp                |
			|           \               >           \               |
			|            prev           |            prevl          |
			|           /               |                           |
			|      prevl                |                           |
			\*--------------------------+---------------------------*/

			MCF_AvlNodeHeader *const pPrevParent = pPrev->pParent;
			MCF_AvlNodeHeader **const ppPrevRefl = pPrev->ppRefl;
			MCF_AvlNodeHeader *const pPrevLeft = pPrev->pLeft;

			ASSERT(pPrevParent && (pPrevParent != pNode));

			*ppRefl = pPrev;

			pPrev->pParent	= pParent;
			pPrev->ppRefl	= ppRefl;
			pPrev->pLeft	= pLeft;
			pPrev->pRight	= pRight;
			pPrev->uHeight	= pNode->uHeight;

			pLeft->pParent = pPrev;
			pLeft->ppRefl = &(pPrev->pLeft);

			if(pRight){
				pRight->pParent = pPrev;
				pRight->ppRefl = &(pPrev->pRight);
			}

			*ppPrevRefl = pPrevLeft;

			if(pPrevLeft){
				pPrevLeft->pParent = pPrevParent;
				pPrevLeft->ppRefl = ppPrevRefl;
			}

			UpdateRecur(pPrevParent);
		}
	}
	if(pPrev){
		pPrev->pNext = pNext;
	}
	if(pNext){
		pNext->pPrev = pPrev;
	}
}

MCF_AvlNodeHeader *MCF_AvlFront(const MCF_AvlRoot *ppRoot){
	MCF_AvlNodeHeader *pCur = *ppRoot;
	if(pCur){
		while(pCur->pLeft){
			pCur = pCur->pLeft;
		}
	}
	return pCur;
}
MCF_AvlNodeHeader *MCF_AvlBack(const MCF_AvlRoot *ppRoot){
	MCF_AvlNodeHeader *pCur = *ppRoot;
	if(pCur){
		while(pCur->pRight){
			pCur = pCur->pRight;
		}
	}
	return pCur;
}

void MCF_AvlSwap(MCF_AvlRoot *ppRoot1, MCF_AvlRoot *ppRoot2){
	if(ppRoot1 == ppRoot2){
		return;
	}

	MCF_AvlNodeHeader *const pRoot1 = *ppRoot1;
	MCF_AvlNodeHeader *const pRoot2 = *ppRoot2;

	*ppRoot2 = pRoot1;
	if(pRoot1){
		pRoot1->ppRefl = ppRoot2;
	}

	*ppRoot1 = pRoot2;
	if(pRoot2){
		pRoot2->ppRefl = ppRoot1;
	}
}
