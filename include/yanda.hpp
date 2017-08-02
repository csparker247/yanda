#pragma once

#include <array>
#include <exception>
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
    using Index = typename Container::size_type;
    /** Extents type */
    using Extent = std::array<Index, N>;
    /** N-Dim Array Index type */
    using ItemIndex = std::array<Index, N>;
    /** Slice type */
    using Slice = NDimensionalArray<T, N - 1>;

    /** @brief Default constructor */
    NDimensionalArray() = default;

    /** @brief Constructor with dimensions */
    explicit NDimensionalArray(Extent e) : extents_(e)
    {
        auto size = std::accumulate(
            e.begin(), e.end(), Index(1), std::multiplies<Index>());

        if (size == 0) {
            throw std::range_error("Array extent is zero");
        }

        data_.resize(size);
    }

    /** @brief Constructor with range initialization */
    NDimensionalArray(
        Extent e,
        typename Container::iterator first,
        typename Container::iterator last)
        : extents_(e), data_{first, last}
    {
        auto size = std::accumulate(
            e.begin(), e.end(), Index(1), std::multiplies<Index>());

        if (size != data_.size()) {
            throw std::range_error("Array extent does not match input data");
        }
    }

    /** @brief Set the extent of the array's dimensions
     *
     * @warning Does not guarantee validity of stored values after resize
     */
    void setExtents(Extent e)
    {
        auto size = std::accumulate(
            e.begin(), e.end(), Index(1), std::multiplies<Index>());

        if (size == 0) {
            throw std::range_error("Array extent is zero");
        }

        extents_ = e;
        data_.resize(size);
    }

    /** @brief Get the extent of the array's dimensions */
    Extent extents() const { return extents_; }

    /** @brief Element access */
    T& operator()(ItemIndex index)
    {
        return data_.at(item_index_to_data_index_(index));
    }

    /** @overload T& operator()(ItemIndex index) */
    const T& operator()(ItemIndex index) const
    {
        return data_.at(item_index_to_data_index_(index));
    }

    /** @brief Get slice of array by dropping highest dimension */
    Slice slice(Index index)
    {
        auto offset = std::accumulate(
            std::next(extents_.begin(), 1), extents_.end(), Index(1),
            std::multiplies<Index>());

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

    /** Convert item index to data index */
    inline Index item_index_to_data_index_(ItemIndex i) const
    {
        Index idx{0};
        for (auto it = 0; it < extents_.size(); it++) {
            auto begin = std::next(extents_.begin(), it + 1);
            auto offset = std::accumulate(
                begin, extents_.end(), Index(1), std::multiplies<Index>());
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
    using Index = typename NDimensionalArray<T, 2>::Index;
    for (Index y = 0; y < a.extents()[0]; y++) {
        std::cout << "[";
        for (Index x = 0; x < a.extents()[1]; x++) {
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
    using Index = typename NDimensionalArray<T, 3>::Index;
    for (Index z = 0; z < a.extents()[0]; z++) {
        std::cout << "[";
        for (Index y = 0; y < a.extents()[1]; y++) {
            std::cout << "[";
            for (Index x = 0; x < a.extents()[2]; x++) {
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