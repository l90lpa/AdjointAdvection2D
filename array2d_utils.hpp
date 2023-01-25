
#include <algorithm>
#include <cassert>
#include <type_traits>

#include "array2d.hpp"
#include "bitmap_image.hpp"

template<typename T>
bitmap_image toBMP(const Array2D<T>& data) {
    static_assert(std::is_floating_point_v<T>);

    auto [min_it, max_it] = std::minmax_element(data.begin(), data.end());
    const auto min_val = *min_it;
    const auto range = *max_it - min_val;

    bitmap_image image(data.rows(), data.columns());
    for (int i = 0; i < data.rows(); i++) {
        for (int j = 0; j < data.columns(); j++) {
            const auto gray = static_cast<unsigned char>(255 * ((data(i, j) - min_val) / range));
            rgb_t colour;
            colour.red = gray;
            colour.green = gray;
            colour.blue = gray;

            image.set_pixel(i, j, colour);
        }
    }

    return image;
}

template<typename T>
Array2D<double> absDiff(const Array2D<T>& v1, const Array2D<T>& v2) {
    static_assert(std::is_arithmetic_v<T>);
    assert(v1.rows() == v2.rows() && v1.columns() == v2.columns());

    Array2D<double> diff(v1.rows(), v1.columns());
    for (int i = 0; i < v1.rows(); i++) {
        for (int j = 0; j < v1.columns(); j++) {
            diff(i, j) = std::abs(v1(i, j) - v2(i, j));
        }
    }
    return diff;
}