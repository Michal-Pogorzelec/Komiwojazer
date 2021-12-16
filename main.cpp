#include "TSP.hpp"

#include <iostream>


int main() {
    cost_matrix_t cm = {{INF, 10, 8,   19, 12},
                      {10, INF, 20,  6,  3},
                      {8,   20, INF, 4,  2},
                      {19,  6,  4, INF,  7},
                      {12,  3,  2,   7, INF}};

//    StageState SS1(cm);
//
//    //redukcja macierzy
//    std::cout << SS1.reduce_cost_matrix() << std::endl;
//    std::cout << SS1.get_matrix() << std::endl;
//
//    //wybór nowego wierzchołka
//    NewVertex new_vert = SS1.choose_new_vertex();
//    //dodanie wierzch. do sciezki (unsorted)
//    SS1.append_to_path(new_vert.coordinates);
//    std::cout << new_vert.coordinates.row + 1<< ", " << new_vert.coordinates.col +1 << std::endl;
//    std::cout << std::endl;
//
//    //aktualizacja macierzy kosztów
//    SS1.update_cost_matrix(new_vert.coordinates);
//
//    std::cout << SS1.get_matrix() << std::endl;
//
//    std::cout << SS1.reduce_cost_matrix() << std::endl;
//    std::cout << SS1.get_matrix() << std::endl;
//
//    new_vert = SS1.choose_new_vertex();
//    SS1.append_to_path(new_vert.coordinates);
//    std::cout << new_vert.coordinates.row + 1<< ", " << new_vert.coordinates.col +1 << std::endl;
//    SS1.update_cost_matrix(new_vert.coordinates);
//
//
//
//    std::cout << SS1.get_matrix() << std::endl;
//    std::cout << SS1.reduce_cost_matrix() << std::endl;
//    std::cout << SS1.get_matrix() << std::endl;
//
//
//    new_vert = SS1.choose_new_vertex();
//    SS1.append_to_path(new_vert.coordinates);
//    SS1.update_cost_matrix(new_vert.coordinates);
//    std::cout << new_vert.coordinates.row + 1<< ", " << new_vert.coordinates.col +1 << std::endl;
//
//    std::cout << SS1.get_matrix() << std::endl;
//
//    for(auto i : SS1.get_unsorted_path()){
//        std::cout << i.row + 1 <<i.col + 1 <<std::endl;
//    }
//
//    auto a = SS1.get_path();
//
//
//    std::cout << SS1.get_matrix() << std::endl;

    tsp_solutions_t result = solve_tsp(cm);


    for (auto elem : result) {
        std::cout << elem.lower_bound << " : ";
        for (auto pelem : elem.path) {
            std::cout << pelem << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";

    /* Rozwiązania:
    * 32 : 3 4 5 2 1
    * 32 : 2 5 4 3 1
    */
    cost_matrix_t cm_2 {
            {INF, 12,   3,  45,   6},
            {78, INF,  90,  21,   3},
            { 5,  56, INF,  23,  98},
            {12,   6,   8, INF,  34},
            { 3,  98,   3,   2, INF}
    };


    tsp_solutions_t result2 = solve_tsp(cm_2);
    for (auto elem : result2) {
        std::cout << elem.lower_bound << " : ";
        for (auto pelem : elem.path) {
            std::cout << pelem << " ";
        }
        std::cout << "\n";
    }
    /* Rozwiązanie:
     * 30 : 4 3 2 0 1
    */

    cost_matrix_t cm3 {
            {INF,  3,  4,  2,  7},
            {3,  INF,  4,  6,  3},
            {4,  4,  INF,  5,  8},
            {2,  6,  5,  INF,  6},
            {7,  3,  8,  6,  INF},
    };


    /* Rozwiązania:
     * 19 : 4 3 0 2 1
     * 19 : 1 2 0 3 4
    */

    tsp_solutions_t solutions = solve_tsp(cm3);


    for (auto elem : solutions) {
        std::cout << elem.lower_bound << " : ";
        for (auto pelem : elem.path) {
            std::cout << pelem << " ";
        }
        std::cout << "\n";
    }

    return EXIT_SUCCESS;
}
