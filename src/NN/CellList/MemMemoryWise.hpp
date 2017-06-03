/*
 * MemMemoryWise.hpp
 *
 *  Created on: Mar 22, 2015
 *  Last modified: June 25, 2015
 *      Authors: Pietro Incardona, Yaroslav Zaluzhnyi
 */

#ifndef CELLISTMEM_HPP_
#define CELLISTMEM_HPP_

#include "CellList.hpp"

/*! \brief Class for MEMORY-WISE cell list implementation
 *
 * This class implement the MEMORY-WISE cell list
 * The memory allocation is small.
 * The memory allocation is (in byte) Size = O(N*size_of(ele))
 *
 * Where
 *
 * N = total number of elements
 * M = number of cells
 * sizeof(ele) = the size of the element the cell list is storing, example if
 *               the cell list store the particle id (64bit) is 8 byte
 *
 * \note It is useful when M >> N
 *
 * \tparam dim Dimensionality of the space
 * \tparam T type of the space float, double, complex
 *
 */
template<unsigned int dim, typename T>
class Mem_mw
{
	typedef openfpm::vector<size_t> base;

	// each cell has a dynamic structure
	// that store the elements in the cell
	std::unordered_map<size_t,base> cl_base;

	typename std::remove_reference<decltype(std::declval<openfpm::vector<size_t>>().get(0))>::type invalid;

	openfpm::vector<size_t> invalid_v;

public:

	// Object type that the structure store
	typedef T value_type;


	inline void init_to_zero(size_t slot, size_t tot_n_cell)
	{
	}

	inline Mem_mw & operator=(const Mem_mw & cell)
	{
		cl_base = cell.cl_base;
		return *this;
	}

	inline void addCell(size_t cell_id, typename base::value_type ele)
	{
		//add another neighbor element

		cl_base[cell_id].add(ele);
	}

	inline void add(size_t cell_id, typename base::value_type ele)
	{
		this->addCell(cell_id,ele);
	}

	inline void remove(size_t cell, size_t ele)
	{
		cl_base[cell].remove(ele);
	}

	inline size_t getNelements(const size_t cell_id) const
	{
		auto it = cl_base.find(cell_id);
		if (it == cl_base.end())
			return 0;

		return it->second.size();
	}

	inline auto get(size_t cell, size_t ele) -> decltype(cl_base[0].get(0)) &
	{
		auto it = cl_base.find(cell);
		if (it == cl_base.end())
			return invalid;

		return it->second.get(ele);
	}

	inline auto get_v(size_t cell) -> decltype(cl_base[0]) &
	{
		auto it = cl_base.find(cell);
		if (it == cl_base.end())
			return invalid_v;

		return it->second;
	}

	inline auto get(size_t cell, size_t ele) const -> const decltype(cl_base.find(cell)->second.get(0)) &
	{
		auto it = cl_base.find(cell);
		if (it == cl_base.end())
			return invalid;

		return it->second.get(ele);
	}

	inline auto get_v(size_t cell) const -> const decltype(cl_base.find(cell)->second) &
	{
		auto it = cl_base.find(cell);
		if (it == cl_base.end())
			return invalid_v;

		return it->second;
	}

	inline void swap(Mem_mw & cl)
	{
		cl_base.swap(cl.cl_base);
	}

	inline void swap(Mem_mw && cell)
	{
		cl_base.swap(cell.cl_base);
	}

	inline void clear()
	{
		cl_base.clear();
	}

	inline const size_t & getStartId(size_t part_id) const
	{
		auto it = cl_base.find(part_id);
		if (it == cl_base.end())
			return *(&invalid_v.get(0));

		return it->second.get(0);
	}

	inline const size_t & getStopId(size_t part_id) const
	{
		auto it = cl_base.find(part_id);
		if (it == cl_base.end())
			return *(&invalid_v.get(0));

		return *(&it->second.last() + 1);
	}

	inline const size_t & get_lin(const size_t * part_id) const
	{
		return *part_id;
	}

public:

	inline Mem_mw(size_t slot)
	{}

	inline void set_slot(size_t slot)
	{}

};


#endif /* CELLISTMEM_HPP_ */