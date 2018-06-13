/**
 * YANDA: Yet Another N-Dimensional Array
 * https://github.com/csparker247/yanda
 */

#pragma once

#include <array>
#include <exception>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

namespace yanda
{
template <typename T, int N>
class NDimensionalArray
{
    static_assert(N > 0, "invalid number of dimensions");

public:
    /** Storage container alias */
    using Container = std::vector<T>;
    /** Container index type */
    using IndexType = typename Container::size_type;
    /** Extents type */
    using Extent = std::array<IndexType, N>;
    /** N-Dim Array Index type */
    using Index = std::array<IndexType, N>;
    /** Slice type */
    using Slice = NDimensionalArray<T, N - 1>;

    /** @brief Default constructor */
    NDimensionalArray() = default;

    /** @brief Constructor with dimensions */
    explicit NDimensionalArray(const Extent& e) : extents_(e)
    {
        resize_container_();
    }

    /** @overload NDimensionalArray(const Extent& e) */
    template <typename... Es>
    explicit NDimensionalArray(Es... extents)
        : extents_{static_cast<IndexType>(extents)...}
    {
        static_assert(sizeof...(extents) == N, "Extents of wrong dimension");
        resize_container_();
    }

    /** @brief Constructor with range initialization */
    NDimensionalArray(
        Extent e,
        typename Container::iterator first,
        typename Container::iterator last)
        : extents_(e), data_{first, last}
    {
        auto size = std::accumulate(
            e.begin(), e.end(), IndexType(1), std::multiplies<IndexType>());

        if (size != data_.size()) {
            throw std::range_error(
                "Array extent does not match size of input data");
        }
    }

    /** @brief Set the extent of the array's dimensions
     *
     * @warning Does not guarantee validity of stored values after resize
     */
    void setExtents(Extent e)
    {
        extents_ = e;
        resize_container_();
    }

    /** @overload void setExtents(Extent e) */
    template <typename... Es>
    void setExtents(Es... extents)
    {
        static_assert(sizeof...(extents) == N, "Extents of wrong dimension");
        return setExtents({static_cast<IndexType>(extents)...});
    }

    /** @brief Get the extent of the array's dimensions */
    Extent extents() const { return extents_; }

    /** @brief Element access */
    T& operator()(Index index) { return data_.at(index_to_data_index_(index)); }

    /** @overload T& operator()(Index index) */
    const T& operator()(Index index) const
    {
        return data_.at(index_to_data_index_(index));
    }

    /** @overload T& operator()(Index index) */
    template <typename... Is>
    T& operator()(Is... indices)
    {
        static_assert(sizeof...(indices) == N, "Index of wrong dimension");
        return operator()(Index{static_cast<IndexType>(indices)...});
    }

    /** @overload T& operator()(Index index) */
    template <typename... Is>
    const T& operator()(Is... indices) const
    {
        static_assert(sizeof...(indices) == N, "Index of wrong dimension");
        return operator()(Index{static_cast<IndexType>(indices)...});
    }

    /** @brief Get slice of array by dropping highest dimension */
    Slice slice(IndexType index)
    {
        auto offset = std::accumulate(
            std::next(extents_.begin(), 1), extents_.end(), IndexType(1),
            std::multiplies<IndexType>());

        auto b = std::next(data_.begin(), index * offset);
        auto e = std::next(data_.begin(), (index + 1) * offset);

        typename Slice::Extent newExtent;
        std::copy(
            std::next(extents_.begin(), 1), extents_.end(), newExtent.begin());

        return Slice(newExtent, b, e);
    }

    /** @brief Return copy of raw data */
    Container data() const { return data_; }

private:
    /** Dimension extents */
    Extent extents_;
    /** Data storage */
    Container data_;

    /** Resize the data container to current extents */
    void resize_container_()
    {
        auto size = std::accumulate(
            extents_.begin(), extents_.end(), IndexType(1),
            std::multiplies<IndexType>());

        if (size == 0) {
            throw std::range_error("Array extent is zero");
        }

        data_.resize(size);
    }

    /** Convert item index to data index */
    inline IndexType index_to_data_index_(Index i) const
    {
        IndexType idx{0};
        for (auto it = 0; it < extents_.size(); it++) {
            auto begin = std::next(extents_.begin(), it + 1);
            auto offset = std::accumulate(
                begin, extents_.end(), IndexType(1),
                std::multiplies<IndexType>());
            idx += i[it] * offset;
        }

        return idx;
    }
};

/* Utilities */
// Print a 2D array
template <typename T>
void Print(const NDimensionalArray<T, 2>& a)
{
    using IndexType = typename NDimensionalArray<T, 2>::IndexType;
    for (IndexType y = 0; y < a.extents()[0]; y++) {
        std::cout << "[";
        for (IndexType x = 0; x < a.extents()[1]; x++) {
            std::cout << a({y, x});
            if (x != a.extents()[1] - 1) {
                std::cout << ",";
            }
        }
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
}

// Print a 3D array
template <typename T>
void Print(const NDimensionalArray<T, 3>& a)
{
    using IndexType = typename NDimensionalArray<T, 3>::IndexType;
    for (IndexType z = 0; z < a.extents()[0]; z++) {
        std::cout << "[";
        for (IndexType y = 0; y < a.extents()[1]; y++) {
            std::cout << "[";
            for (IndexType x = 0; x < a.extents()[2]; x++) {
                std::cout << a({z, y, x});
                if (x != a.extents()[2] - 1) {
                    std::cout << ",";
                }
            }
            std::cout << "]";
            if (y != a.extents()[1] - 1) {
                std::cout << ",";
            }
        }
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
}
}