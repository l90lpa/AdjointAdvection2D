#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "array2d_utils.hpp"

const int Nx = 100;
const int Ny = 100;
const double dx = 1.0 / (Nx - 1);
const double dy = 1.0 / (Ny - 1);
const double dt = 0.00005;
const int tmax = 20000;
const double c = 1.0;

void FTCSStep(Array2D<double>& unew, const Array2D<double>& u) {
    assert(u.rows() == unew.rows() && u.columns() == unew.columns() && u.rows() == Nx && u.columns() == Ny);

    for (int i = 0; i < Nx; i++) {
        for (int j = 0; j < Ny; j++) {
            int i_prev = (i + Nx - 1) % Nx;
            int i_next = (i + 1) % Nx;
            int j_prev = (j + Ny - 1) % Ny;
            int j_next = (j + 1) % Ny;
            unew(i, j) = u(i, j) - 0.5 * c * dt / dx * (u(i_next, j) - u(i_prev, j))
                - 0.5 * c * dt / dy * (u(i, j_next) - u(i, j_prev));
        }
    }
}

void LaxWendroffStep(Array2D<double>& unew, const Array2D<double>& u) {
    assert(u.rows() == unew.rows() && u.columns() == unew.columns() && u.rows() == Nx && u.columns() == Ny);

    const double dtdx{ dt / dx };
    const double dtdy{ dt / dy };
    const double c2{ std::pow(c, 2) };
    const double dtdx2{ std::pow(dtdx, 2) };
    const double dtdy2{ std::pow(dtdy, 2) };
    const double dt2dxdy{ std::pow(dt, 2) / (dx * dy) };

    for (int i = 0; i < Nx; i++) {
        for (int j = 0; j < Ny; j++) {
            int i_prev = (i + Nx - 1) % Nx;
            int i_next = (i + 1) % Nx;
            int j_prev = (j + Ny - 1) % Ny;
            int j_next = (j + 1) % Ny;

            unew(i, j) = u(i, j) 
                - 0.5 * c * dtdx * (u(i_next, j) - u(i_prev, j))
                - 0.5 * c * dtdy * (u(i, j_next) - u(i, j_prev))
                + 0.5 * c2 * dtdx2 * (u(i_prev, j) - 2 * u(i, j) + u(i_next, j))
                + 0.5 * c2 * dtdy2 * (u(i, j_prev) - 2 * u(i, j) + u(i, j_next))
                + (1/8) * 2 * c2 * dt2dxdy * ((u(i_next, j_next) - u(i_prev, j_next)) - (u(i_next, j_prev) - u(i_prev, j_prev)));
        }
    }
}


int main() {

    // Initialize u
    Array2D<double> u((size_t)Nx,(size_t)Ny);
    for (int i = 0; i < Nx; i++) {
        for (int j = 0; j < Ny; j++) {
            u(i,j) = sin(2*M_PI*i*dx)*sin(2*M_PI*j*dy);
        }
    }
    const auto u0 = u;
    toBMP(u0).save_image("input.bmp");

    // Solver
    Array2D<double> unew((size_t)Nx,(size_t)Ny);
    for (int t = 0; t < tmax; t++) {
        LaxWendroffStep(unew, u);
        u = unew;
    }

    const auto diff = absDiff(u, u0);
    const auto max_it = std::max_element(diff.begin(), diff.end());
    std::cout << "Max absolute difference = " << *max_it << std::endl;

    toBMP(u).save_image("output.bmp");
    toBMP(diff).save_image("diff.bmp");

    return 0;
}