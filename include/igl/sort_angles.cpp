#include "sort_angles.h"
#include <algorithm>

template <typename DerivedM, typename DerivedR>
IGL_INLINE void igl::sort_angles(
        const Eigen::PlainObjectBase<DerivedM>& M,
        Eigen::PlainObjectBase<DerivedR>& R) {
    const size_t num_rows = M.rows();
    const size_t num_cols = M.cols();
    assert(num_cols >= 2);

    R.resize(num_rows);
    R.setLinSpaced(num_rows, 0, num_rows-1);

    //              |
    // (pi/2, pi)   | (0, pi/2)
    //              |
    // -------------+--------------
    //              |
    // (-pi, -pi/2) | (-pi/2, 0)
    //              |
    auto comp = [&](size_t i, size_t j) {
        auto yi = M(i, 0);
        auto xi = M(i, 1);
        auto yj = M(j, 0);
        auto xj = M(j, 1);

        if (xi == xj && yi == yj) {
            for (size_t idx=2; idx<num_cols; idx++) {
                auto i_val = M(i, idx);
                auto j_val = M(j, idx);
                if (i_val != j_val) {
                    return i_val < j_val;
                }
            }
            // If the entire rows are equal, use the row index.
            return i < j;
        }

        if (xi >= 0 && yi >= 0) {
            if (xj >=0 && yj >= 0) {
                if (xi != xj) {
                    return xi > xj;
                } else {
                    return yi < yj;
                }
            } else if (xj < 0 && yj >= 0) {
                return true;
            } else if (xj < 0 && yj < 0) {
                return false;
            } else {
                return false;
            }
        } else if (xi < 0 && yi >= 0) {
            if (xj >= 0 && yj >= 0) {
                return false;
            } else if (xj < 0 && yj >= 0) {
                if (xi != xj) {
                    return xi > xj;
                } else {
                    return yi > yj;
                }
            } else if (xj < 0 && yj < 0) {
                return false;
            } else {
                return false;
            }
        } else if (xi < 0 && yi < 0) {
            if (xj >= 0 && yj >= 0) {
                return true;
            } else if (xj < 0 && yj >= 0) {
                return true;
            } else if (xj < 0 && yj < 0) {
                if (xi != xj) {
                    return xi < xj;
                } else {
                    return yi > yj;
                }
            } else {
                return true;
            }
        } else {
            if (xj >= 0 && yj >= 0) {
                return true;
            } else if (xj < 0 && yj >= 0) {
                return true;
            } else if (xj < 0 && yj < 0) {
                return false;
            } else {
                if (xi != xj) {
                    return xi < xj;
                } else {
                    return yi < yj;
                }
            }
        }
    };
    std::sort(R.data(), R.data() + num_rows, comp);
}
