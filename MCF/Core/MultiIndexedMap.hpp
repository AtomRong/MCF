// 这个文件是 MCF 的一部分。
// 有关具体授权说明，请参阅 MCFLicense.txt。
// Copyleft 2014. LH_Mouse. All wrongs reserved.

#ifndef MCF_MULTI_INDEXED_HPP_
#define MCF_MULTI_INDEXED_HPP_

#include "../../MCFCRT/ext/offset_of.h"
#include "../../MCFCRT/env/avl_tree.h"
#include "Utilities.hpp"
#include "VVector.hpp"
#include <tuple>
#include <type_traits>
#include <functional>

namespace MCF {

// 这并不是真正的索引，因为它不能用来检索元素，而只能用来遍历。
// 但是这个索引保证了元素创建的顺序。
struct SequenceIndex {
};

template<class Element_t, class... Indices_t>
class MultiIndexedMap;

template<class Element_t, class... Indices_t>
class MultiIndexedMapNode {
	static_assert(sizeof...(Indices_t) > 0, "No index?");

	template<class, class...>
	friend class MultiIndexedMap;

private:
	typedef std::tuple<Indices_t...> xIndexTuple;

private:
	Element_t xm_vElement;
	xIndexTuple xm_vIndices;
	MCF_AVL_NODE_HEADER xm_aHeaders[sizeof...(Indices_t)];

	const MultiIndexedMapNode *xm_pReserved; // 复制树结构的时候用到。
	MCF_AVL_NODE_HEADER xm_vReservedHeader;

public:
	explicit constexpr MultiIndexedMapNode(Element_t vElement, Indices_t... vIndices)
		: xm_vElement(std::move(vElement))
		, xm_vIndices(std::move(vIndices)...)
		, xm_aHeaders()
	{
	}

public:
	constexpr const Element_t &GetElement() const noexcept {
		return xm_vElement;
	}
	Element_t &GetElement() noexcept {
		return xm_vElement;
	}
	template<std::size_t INDEX>
	constexpr auto &GetIndex() const noexcept {
		return std::get<INDEX>(xm_vIndices);
	}

	template<std::size_t INDEX>
	const MultiIndexedMapNode *GetPrev() const noexcept {
		const auto pPrevAvl = ::MCF_AvlPrev(xm_aHeaders + INDEX);
		if(!pPrevAvl){
			return nullptr;
		}
		return DOWN_CAST(const MultiIndexedMapNode, xm_aHeaders[INDEX], pPrevAvl);
	}
	template<std::size_t INDEX>
	MultiIndexedMapNode *GetPrev() noexcept {
		const auto pPrevAvl = ::MCF_AvlPrev(xm_aHeaders + INDEX);
		if(!pPrevAvl){
			return nullptr;
		}
		return DOWN_CAST(MultiIndexedMapNode, xm_aHeaders[INDEX], pPrevAvl);
	}

	template<std::size_t INDEX>
	const MultiIndexedMapNode *GetNext() const noexcept {
		const auto pNextAvl = ::MCF_AvlNext(xm_aHeaders + INDEX);
		if(!pNextAvl){
			return nullptr;
		}
		return DOWN_CAST(const MultiIndexedMapNode, xm_aHeaders[INDEX], pNextAvl);
	}
	template<std::size_t INDEX>
	MultiIndexedMapNode *GetNext() noexcept {
		const auto pNextAvl = ::MCF_AvlNext(xm_aHeaders + INDEX);
		if(!pNextAvl){
			return nullptr;
		}
		return DOWN_CAST(MultiIndexedMapNode, xm_aHeaders[INDEX], pNextAvl);
	}
};

template<class... Indices_t>
class MultiIndexedMapNode<void, Indices_t...> {
	static_assert(sizeof...(Indices_t) > 0, "No index?");

	template<class, class...>
	friend class MultiIndexedMap;

private:
	typedef std::tuple<Indices_t...> xIndexTuple;

private:
	xIndexTuple xm_vIndices;
	MCF_AVL_NODE_HEADER xm_aHeaders[sizeof...(Indices_t)];

	const MultiIndexedMapNode *xm_pReserved; // 复制树结构的时候用到。
	MCF_AVL_NODE_HEADER xm_vReservedHeader;

public:
	explicit constexpr MultiIndexedMapNode(Indices_t... vIndices)
		: xm_vIndices(std::move(vIndices)...)
		, xm_aHeaders()
	{
	}

public:
	template<std::size_t INDEX>
	constexpr auto &GetIndex() const noexcept {
		return std::get<INDEX>(xm_vIndices);
	}

	template<std::size_t INDEX>
	const MultiIndexedMapNode *GetPrev() const noexcept {
		const auto pPrevAvl = ::MCF_AvlPrev(xm_aHeaders + INDEX);
		if(!pPrevAvl){
			return nullptr;
		}
		return DOWN_CAST(const MultiIndexedMapNode, xm_aHeaders[INDEX], pPrevAvl);
	}
	template<std::size_t INDEX>
	MultiIndexedMapNode *GetPrev() noexcept {
		const auto pPrevAvl = ::MCF_AvlPrev(xm_aHeaders + INDEX);
		if(!pPrevAvl){
			return nullptr;
		}
		return DOWN_CAST(MultiIndexedMapNode, xm_aHeaders[INDEX], pPrevAvl);
	}

	template<std::size_t INDEX>
	const MultiIndexedMapNode *GetNext() const noexcept {
		const auto pNextAvl = ::MCF_AvlNext(xm_aHeaders + INDEX);
		if(!pNextAvl){
			return nullptr;
		}
		return DOWN_CAST(const MultiIndexedMapNode, xm_aHeaders[INDEX], pNextAvl);
	}
	template<std::size_t INDEX>
	MultiIndexedMapNode *GetNext() noexcept {
		const auto pNextAvl = ::MCF_AvlNext(xm_aHeaders + INDEX);
		if(!pNextAvl){
			return nullptr;
		}
		return DOWN_CAST(MultiIndexedMapNode, xm_aHeaders[INDEX], pNextAvl);
	}
};

template<class Element_t, class... Indices_t>
class MultiIndexedMap {
	static_assert(sizeof...(Indices_t) > 0, "No index?");

public:
	typedef MultiIndexedMapNode<Element_t, Indices_t...> Node;

private:
	struct xBstNodes {
		MCF_AVL_ROOT pRoot;
		MCF_AVL_NODE_HEADER *pFirst;
		MCF_AVL_NODE_HEADER *pLast;
	};

	template<std::size_t INDEX>
	struct xComparators {
		typedef typename Node::xIndexTuple IndexTuple;
		typedef typename std::tuple_element<INDEX, IndexTuple>::type IndexType;

		static bool Nodes(const MCF_AVL_NODE_HEADER *pAvl1, const MCF_AVL_NODE_HEADER *pAvl2) noexcept {
			const auto pNode1 = DOWN_CAST(const Node, xm_aHeaders[INDEX], pAvl1);
			const auto pNode2 = DOWN_CAST(const Node, xm_aHeaders[INDEX], pAvl2);

		ASSERT_NOEXCEPT_BEGIN
			return std::less<void>()(std::get<INDEX>(pNode1->xm_vIndices), std::get<INDEX>(pNode2->xm_vIndices));
		ASSERT_NOEXCEPT_END
		}
		template<typename Other_t>
		static bool NodeOther(const MCF_AVL_NODE_HEADER *pAvl1, std::intptr_t nOther) noexcept {
			const auto pNode1 = DOWN_CAST(const Node, xm_aHeaders[INDEX], pAvl1);

		ASSERT_NOEXCEPT_BEGIN
			return std::less<void>()(std::get<INDEX>(pNode1->xm_vIndices), *(const Other_t *)nOther);
		ASSERT_NOEXCEPT_END
		}
		template<typename Other_t>
		static bool OtherNode(std::intptr_t nOther, const MCF_AVL_NODE_HEADER *pAvl2) noexcept {
			const auto pNode2 = DOWN_CAST(const Node, xm_aHeaders[INDEX], pAvl2);

		ASSERT_NOEXCEPT_BEGIN
			return std::less<void>()(*(const Other_t *)nOther, std::get<INDEX>(pNode2->xm_vIndices));
		ASSERT_NOEXCEPT_END
		}
	};

	struct xAddressComparators {
		static bool Nodes(const MCF_AVL_NODE_HEADER *pAvl1, const MCF_AVL_NODE_HEADER *pAvl2) noexcept {
			const auto pNode1 = DOWN_CAST(const Node, xm_vReservedHeader, pAvl1);
			const auto pNode2 = DOWN_CAST(const Node, xm_vReservedHeader, pAvl2);
			return std::less<void>()(pNode1->xm_pReserved, pNode2->xm_pReserved);
		}
		static bool NodeOther(const MCF_AVL_NODE_HEADER *pAvl1, std::intptr_t nOther) noexcept {
			const auto pNode1 = DOWN_CAST(const Node, xm_vReservedHeader, pAvl1);
			return std::less<void>()(pNode1->xm_pReserved, (const Node *)nOther);
		}
		static bool OtherNode(std::intptr_t nOther, const MCF_AVL_NODE_HEADER *pAvl2) noexcept {
			const auto pNode2 = DOWN_CAST(const Node, xm_vReservedHeader, pAvl2);
			return std::less<void>()((const Node *)nOther, pNode2->xm_pReserved);
		}
	};

private:
	xBstNodes xm_aNodes[sizeof...(Indices_t)];

	std::size_t xm_uSize;

public:
	constexpr MultiIndexedMap() noexcept
		: xm_aNodes()
		, xm_uSize(0)
	{
	}
	MultiIndexedMap(const MultiIndexedMap &rhs)
		: MultiIndexedMap()
	{
		// 稳定地复制树结构。

		// 第一步，分配全部新节点。
		MCF_AVL_ROOT pavlNodeMap = nullptr;
		try {
			auto pNode = rhs.GetBegin<0>();
			while(pNode){
				const auto pNewNode = new Node(*pNode);
				pNewNode->xm_pReserved = pNode; // 放置源节点的地址。
				::MCF_AvlAttach(
					&pavlNodeMap,
					&(pNewNode->xm_vReservedHeader),
					&(xAddressComparators::Nodes)
				);
				pNode = pNode->template GetNext<0>();
			}
		} catch(...){
			if(pavlNodeMap){
				auto pCur = ::MCF_AvlPrev(pavlNodeMap);
				while(pCur){
					const auto pPrev = ::MCF_AvlPrev(pCur);
					delete DOWN_CAST(Node, xm_vReservedHeader, pCur);
					pCur = pPrev;
				}
				pCur = ::MCF_AvlNext(pavlNodeMap);
				while(pCur){
					const auto pNext = ::MCF_AvlNext(pCur);
					delete DOWN_CAST(Node, xm_vReservedHeader, pCur);
					pCur = pNext;
				}
				delete DOWN_CAST(Node, xm_vReservedHeader, pavlNodeMap);
			}
			throw;
		}

		// 所有节点都分配完成，现在进行第二步，重建每个二叉树，保证无异常抛出。
		xCloneTreeRecur<0>(rhs.xm_aNodes, pavlNodeMap);

		// 最后一步，填写 size。
		xm_uSize = rhs.xm_uSize;
	}
	MultiIndexedMap(MultiIndexedMap &&rhs) noexcept
		: MultiIndexedMap()
	{
		Swap(rhs);
	}
	MultiIndexedMap &operator=(const MultiIndexedMap &rhs){
		MultiIndexedMap(rhs).Swap(*this);
		return *this;
	}
	MultiIndexedMap &operator=(MultiIndexedMap &&rhs) noexcept {
		rhs.Swap(*this);
		return *this;
	}
	~MultiIndexedMap() noexcept {
		Clear();
	}

private:
	template<std::size_t INDEX>
	void xAttach(
		Node *pNode,
		typename std::enable_if<
			!std::is_same<
				typename std::tuple_element<INDEX, std::tuple<Indices_t...>>::type,
				SequenceIndex
			>::value,
			int
		>::type = 0
	) noexcept {
		const auto pAvl = pNode->xm_aHeaders + INDEX;

		::MCF_AvlAttach(
			&(xm_aNodes[INDEX].pRoot),
			pAvl,
			&(xComparators<INDEX>::Nodes)
		);

		if(!::MCF_AvlPrev(pAvl)){
			xm_aNodes[INDEX].pFirst = pAvl;
		}
		if(!::MCF_AvlNext(pAvl)){
			xm_aNodes[INDEX].pLast = pAvl;
		}
	}
	template<std::size_t INDEX>
	void xDetach(
		Node *pNode,
		typename std::enable_if<
			!std::is_same<
				typename std::tuple_element<INDEX, std::tuple<Indices_t...>>::type,
				SequenceIndex
			>::value,
			int
		>::type = 0
	) noexcept {
		const auto pAvl = pNode->xm_aHeaders + INDEX;

		if(pAvl == xm_aNodes[INDEX].pFirst){
			xm_aNodes[INDEX].pFirst = ::MCF_AvlNext(pAvl);
		}
		if(pAvl == xm_aNodes[INDEX].pLast){
			xm_aNodes[INDEX].pLast = ::MCF_AvlPrev(pAvl);
		}

		::MCF_AvlDetach(pAvl);
	}

	template<std::size_t INDEX>
	void xAttach(
		Node *pNode,
		typename std::enable_if<
			std::is_same<
				typename std::tuple_element<INDEX, std::tuple<Indices_t...>>::type,
				SequenceIndex
			>::value,
			int
		>::type = 0
	) noexcept {
		const auto pAvl = pNode->xm_aHeaders + INDEX;

		auto &pLast = xm_aNodes[INDEX].pLast;
		pAvl->pPrev = pLast;
		pAvl->pNext = nullptr;

		if(pLast){
			pLast->pNext = pAvl;
		} else {
			xm_aNodes[INDEX].pFirst = pAvl;
		}
		pLast = pAvl;
	}
	template<std::size_t INDEX>
	void xDetach(
		Node *pNode,
		typename std::enable_if<
			std::is_same<
				typename std::tuple_element<INDEX, std::tuple<Indices_t...>>::type,
				SequenceIndex
			>::value,
			int
		>::type = 0
	) noexcept {
		const auto pAvl = pNode->xm_aHeaders + INDEX;

		const auto pPrev = ::MCF_AvlPrev(pAvl);
		const auto pNext = ::MCF_AvlNext(pAvl);
		if(pPrev){
			pPrev->pNext = pNext;
		}
		if(pNext){
			pNext->pPrev = pPrev;
		}

		if(pAvl == xm_aNodes[INDEX].pFirst){
			xm_aNodes[INDEX].pFirst = pNext;
		}
		if(pAvl == xm_aNodes[INDEX].pLast){
			xm_aNodes[INDEX].pLast = pPrev;
		}
	}

	template<std::size_t INDEX>
	void xAttachRecur(
		Node *pNode,
		typename std::enable_if<(INDEX < sizeof...(Indices_t)), int>::type = 0
	) noexcept {
		xAttach<INDEX>(pNode);
		xAttachRecur<INDEX + 1>(pNode);
	}
	template<std::size_t INDEX>
	void xAttachRecur(
		Node *,
		typename std::enable_if<(INDEX == sizeof...(Indices_t)), int>::type = 0
	) noexcept {
	}

	template<std::size_t INDEX>
	void xDetachRecur(
		Node *pNode,
		typename std::enable_if<(INDEX < sizeof...(Indices_t)), int>::type = 0
	) noexcept {
		xDetachRecur<INDEX + 1>(pNode);
		xDetach<INDEX>(pNode);
	}
	template<std::size_t INDEX>
	void xDetachRecur(
		Node *pNode,
		typename std::enable_if<(INDEX == sizeof...(Indices_t)), int>::type = 0
	) noexcept {
#ifdef NDEBUG
		(void)pNode;
#else
		__builtin_memset(&(pNode->xm_aHeaders), 0xCD, sizeof((pNode->xm_aHeaders)));
#endif
	}

	template<std::size_t INDEX>
	void xCloneTreeRecur(
		const xBstNodes *pSrcNodes,
		const MCF_AVL_ROOT &pavlAllocatedNodes,
		typename std::enable_if<(INDEX < sizeof...(Indices_t)), int>::type = 0
	) noexcept {
		for(auto pSrc = pSrcNodes[INDEX].pFirst; pSrc; pSrc = ::MCF_AvlNext(pSrc)){
			const auto pNewAvl = ::MCF_AvlFind(
				&pavlAllocatedNodes,
				(std::intptr_t)DOWN_CAST(const Node, xm_aHeaders[INDEX], pSrc),
				&(xAddressComparators::NodeOther),
				&(xAddressComparators::OtherNode)
			);
			ASSERT(pNewAvl);

			xAttach<INDEX>(DOWN_CAST(Node, xm_vReservedHeader, pNewAvl));
		}
		xCloneTreeRecur<INDEX + 1>(pSrcNodes, pavlAllocatedNodes);
	}
	template<std::size_t INDEX>
	void xCloneTreeRecur(
		const xBstNodes *,
		const MCF_AVL_ROOT &,
		typename std::enable_if<(INDEX == sizeof...(Indices_t)), int>::type = 0
	) noexcept {
	}

public:
	template<typename... Params_t>
	Node *Insert(Params_t &&... vParams){
		const auto pNode = new Node(std::forward<Params_t>(vParams)...);
		xAttachRecur<0>(pNode);
		++xm_uSize;
		return pNode;
	}
	void Erase(Node *pNode) noexcept {
		xDetachRecur<0>(pNode);
		--xm_uSize;
		delete pNode;
	}
	void Clear() noexcept {
		auto pNode = GetBegin<0>();
		while(pNode){
			const auto pNext = pNode->template GetNext<0>();
			delete pNode;
			pNode = pNext;
		}

		BZero(xm_aNodes);
		xm_uSize = 0;
	}

	std::size_t GetSize() const noexcept {
		return xm_uSize;
	}
	bool IsEmpty() const noexcept {
		return xm_uSize == 0;
	}

	void Swap(MultiIndexedMap &rhs) noexcept {
		if(this != &rhs){
			for(std::size_t i = 0; i < sizeof...(Indices_t); ++i){
				::MCF_AvlSwap	(&(xm_aNodes[i].pRoot),	&(rhs.xm_aNodes[i].pRoot));
				std::swap		(xm_aNodes[i].pFirst,	rhs.xm_aNodes[i].pFirst);
				std::swap		(xm_aNodes[i].pLast,	rhs.xm_aNodes[i].pLast);
			}
			std::swap(xm_uSize, rhs.xm_uSize);
		}
	}

	template<std::size_t INDEX, typename... Params_t>
	void SetIndex(Node *pNode, Params_t &&... vParams)
		noexcept(
			std::is_nothrow_constructible<
				typename std::tuple_element<INDEX, typename Node::xIndexTuple>::type,
				Params_t...
			>::value &&
			std::is_nothrow_move_assignable<
				typename std::tuple_element<INDEX, typename Node::xIndexTuple>::type
			>::value
		)
	{
		typename std::tuple_element<INDEX, typename Node::xIndexTuple>::type
			vNewIndex(std::forward<Params_t>(vParams)...);

		xDetach<INDEX>(pNode);
		try {
			std::get<INDEX>(pNode->xm_vIndices) = std::move(vNewIndex);
			xAttach<INDEX>(pNode);
		} catch(...){
			xAttach<INDEX>(pNode);
			throw;
		}
	}

	template<std::size_t INDEX>
	const Node *GetBegin() const noexcept {
		const auto pAvl = xm_aNodes[INDEX].pFirst;
		if(!pAvl){
			return nullptr;
		}
		return DOWN_CAST(const Node, xm_aHeaders[INDEX], pAvl);
	}
	template<std::size_t INDEX>
	Node *GetBegin() noexcept {
		const auto pAvl = xm_aNodes[INDEX].pFirst;
		if(!pAvl){
			return nullptr;
		}
		return DOWN_CAST(Node, xm_aHeaders[INDEX], pAvl);
	}
	template<std::size_t INDEX>
	const Node *GetCBegin() const noexcept {
		return GetBegin();
	}

	template<std::size_t INDEX>
	const Node *GetRBegin() const noexcept {
		const auto pAvl = xm_aNodes[INDEX].pLast;
		if(!pAvl){
			return nullptr;
		}
		return DOWN_CAST(const Node, xm_aHeaders[INDEX], pAvl);
	}
	template<std::size_t INDEX>
	Node *GetRBegin() noexcept {
		const auto pAvl = xm_aNodes[INDEX].pLast;
		if(!pAvl){
			return nullptr;
		}
		return DOWN_CAST(Node, xm_aHeaders[INDEX], pAvl);
	}
	template<std::size_t INDEX>
	const Node *GetCRBegin() const noexcept {
		return GetRBegin();
	}

	template<std::size_t INDEX, typename Comparand_t>
	const Node *GetLowerBound(const Comparand_t &vComparand) const noexcept {
		const auto pAvl = ::MCF_AvlLowerBound(
			&(xm_aNodes[INDEX].pRoot),
			(std::intptr_t)&vComparand,
			&(xComparators<INDEX>::template NodeOther<Comparand_t>)
		);
		return pAvl ? DOWN_CAST(const Node, xm_aHeaders[INDEX], pAvl) : nullptr;
	}
	template<std::size_t INDEX, typename Comparand_t>
	Node *GetLowerBound(const Comparand_t &vComparand) noexcept {
		const auto pAvl = ::MCF_AvlLowerBound(
			&(xm_aNodes[INDEX].pRoot),
			(std::intptr_t)&vComparand,
			&(xComparators<INDEX>::template NodeOther<Comparand_t>)
		);
		return pAvl ? DOWN_CAST(Node, xm_aHeaders[INDEX], pAvl) : nullptr;
	}

	template<std::size_t INDEX, typename Comparand_t>
	const Node *GetUpperBound(const Comparand_t &vComparand) const noexcept {
		const auto pAvl = ::MCF_AvlUpperBound(
			&(xm_aNodes[INDEX].pRoot),
			(std::intptr_t)&vComparand,
			&(xComparators<INDEX>::template OtherNode<Comparand_t>)
		);
		return pAvl ? DOWN_CAST(const Node, xm_aHeaders[INDEX], pAvl) : nullptr;
	}
	template<std::size_t INDEX, typename Comparand_t>
	Node *GetUpperBound(const Comparand_t &vComparand) noexcept {
		const auto pAvl = ::MCF_AvlUpperBound(
			&(xm_aNodes[INDEX].pRoot),
			(std::intptr_t)&vComparand,
			&(xComparators<INDEX>::template OtherNode<Comparand_t>)
		);
		return pAvl ? DOWN_CAST(Node, xm_aHeaders[INDEX], pAvl) : nullptr;
	}

	template<std::size_t INDEX, typename Comparand_t>
	const Node *Find(const Comparand_t &vComparand) const noexcept {
		const auto pAvl = ::MCF_AvlFind(
			&(xm_aNodes[INDEX].pRoot),
			(std::intptr_t)&vComparand,
			&(xComparators<INDEX>::template NodeOther<Comparand_t>),
			&(xComparators<INDEX>::template OtherNode<Comparand_t>)
		);
		return pAvl ? DOWN_CAST(const Node, xm_aHeaders[INDEX], pAvl) : nullptr;
	}
	template<std::size_t INDEX, typename Comparand_t>
	Node *Find(const Comparand_t &vComparand) noexcept {
		const auto pAvl = ::MCF_AvlFind(
			&(xm_aNodes[INDEX].pRoot),
			(std::intptr_t)&vComparand,
			&(xComparators<INDEX>::template NodeOther<Comparand_t>),
			&(xComparators<INDEX>::template OtherNode<Comparand_t>)
		);
		return pAvl ? DOWN_CAST(Node, xm_aHeaders[INDEX], pAvl) : nullptr;
	}

	template<std::size_t INDEX, typename Comparand_t>
	std::pair<const Node *, const Node *> GetEqualRange(const Comparand_t &vComparand) const noexcept {
		MCF_AVL_NODE_HEADER *pBegin, *pEnd;
		::MCF_AvlEqualRange(
			&pBegin,
			&pEnd,
			&(xm_aNodes[INDEX].pRoot),
			(std::intptr_t)&vComparand,
			&(xComparators<INDEX>::template NodeOther<Comparand_t>),
			&(xComparators<INDEX>::template OtherNode<Comparand_t>)
		);
		return std::make_pair(
			pBegin ? DOWN_CAST(const Node, xm_aHeaders[INDEX], pBegin) : nullptr,
			pEnd ? DOWN_CAST(const Node, xm_aHeaders[INDEX], pEnd) : nullptr
		);
	}
	template<std::size_t INDEX, typename Comparand_t>
	std::pair<Node *, Node *> GetEqualRange(const Comparand_t &vComparand) noexcept {
		MCF_AVL_NODE_HEADER *pBegin, *pEnd;
		::MCF_AvlEqualRange(
			&pBegin,
			&pEnd,
			&(xm_aNodes[INDEX].pRoot),
			(std::intptr_t)&vComparand,
			&(xComparators<INDEX>::template NodeOther<Comparand_t>),
			&(xComparators<INDEX>::template OtherNode<Comparand_t>)
		);
		return std::make_pair(
			pBegin ? DOWN_CAST(Node, xm_aHeaders[INDEX], pBegin) : nullptr,
			pEnd ? DOWN_CAST(Node, xm_aHeaders[INDEX], pEnd) : nullptr
		);
	}
};

template<class... Indices_t>
using MultiIndexedSet = MultiIndexedMap<void, Indices_t...>;

}

#endif
