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
    using DataIndex = typename Container::size_type;
    /** Extents type */
    using Extent = std::array<DataIndex, N>;
    /** N-Dim Array Index type */
    using ItemIndex = std::array<DataIndex, N>;
    /** Slice type */
    using Slice = NDimensionalArray<T, N - 1>;

    /** @brief Default constructor */
    NDimensionalArray() = default;

    /** @brief Constructor with dimensions */
    explicit NDimensionalArray(Extent e) : extents_(e)
    {
        auto size = std::accumulate(
            e.begin(), e.end(), DataIndex(1), std::multiplies<DataIndex>());

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
    }

    /** @brief Set the extent of the array's dimensions
     *
     * @warning Does not guarantee validity of stored values after resize
     */
    void setExtents(Extent e)
    {
        extents_ = e;

        auto size = std::accumulate(
            e.begin(), e.end(), DataIndex(1), std::multiplies<DataIndex>());

        if (size == 0) {
            throw std::range_error("Array extent is zero");
        }

        data_.resize(size);
    }

    /** @brief Get the extent of the array's dimensions */
    Extent extents() const { return extents_; }

    /** @brief Element access */
    T& operator()(ItemIndex index)
    {
        DataIndex dIdx = 0;
        for (auto it = 0; it < extents_.size(); it++) {
            auto begin = std::next(extents_.begin(), it + 1);
            auto offset = std::accumulate(
                begin, extents_.end(), DataIndex(1),
                std::multiplies<DataIndex>());
            dIdx += index[it] * offset;
        }

        return data_.at(dIdx);
    }

    /** @brief Get slice of array by dropping highest dimension */
    Slice slice(DataIndex index)
    {
        auto offset = std::accumulate(
            std::next(extents_.begin(), 1), extents_.end(), DataIndex(1),
            std::multiplies<DataIndex>());

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
};
}