#include "TSP.hpp"

#include <algorithm>
#include <stack>
#include <optional>
#include <climits>
#include <iostream>

std::ostream& operator<<(std::ostream& os, const CostMatrix& cm) {
    for (std::size_t r = 0; r < cm.size(); ++r) {
        for (std::size_t c = 0; c < cm.size(); ++c) {
            const auto& elem = cm[r][c];
            os << (is_inf(elem) ? "INF" : std::to_string(elem)) << " ";
        }
        os << "\n";
    }
    os << std::endl;

    return os;
}

/* PART 1 */

/**
 * Create path from unsorted path and last 2x2 cost matrix.
 * @return The vector of consecutive vertex.
 */
path_t StageState::get_path() {

    auto matrix=get_matrix();

    // pobranie z macierzy punktów które nie są inf
    std::vector<vertex_t> non_inf_points;
    for (std::size_t i = 0; i < matrix.size(); ++i)
        for (std::size_t j = 0; j < matrix.size(); ++j)
            if(matrix[i][j] != INF)
            {
                non_inf_points.push_back(vertex_t(i, j));
            }

    // sprawdzenie cykli
    std::size_t row_to_del;
    std::size_t col_to_del;

    // szukanie indeksu wiersza do usunięcia
    if(non_inf_points[0].row == non_inf_points[1].row or non_inf_points[2].row == non_inf_points[1].row)
        row_to_del=non_inf_points[1].row;
    else
        row_to_del=non_inf_points[0].row;

    // szukanie indeksu kolumny do usuniecia
    if(non_inf_points[0].col == non_inf_points[1].col or non_inf_points[2].col == non_inf_points[1].col)
        col_to_del=non_inf_points[1].col;
    else
        col_to_del=non_inf_points[0].col;
    matrix[row_to_del][col_to_del]=INF;

    // zsumowanie niezerowych elementów w macierzy i dodanie wartości do lower_bound_
    int sum_of_non_zero_elem = 0;
    auto unsorted = unsorted_path_;
    for (std::size_t i = 0; i < matrix.size(); ++i)
        for (std::size_t j = 0; j < matrix.size(); ++j)
            if(matrix[i][j] != INF)
            {
                sum_of_non_zero_elem += matrix[i][j];
                unsorted.push_back(vertex_t(i,j));
            }
    lower_bound_ += sum_of_non_zero_elem;

    path_t found_path;
    found_path.push_back(unsorted[0].row);
    while(found_path.size() < unsorted.size())
        for(std::size_t i=0;i<unsorted.size() and found_path.size() < unsorted.size(); i++)
            if(unsorted[i].row == found_path[found_path.size() - 1])
                found_path.push_back(unsorted[i].col);

    for(auto &el:found_path)
        el++;

    return found_path;
}

/**
 * Get minimum values from each row and returns them.
 * @return Vector of minimum values in row.
 */
std::vector<cost_t> CostMatrix::get_min_values_in_rows() const {
    std::vector<cost_t> min_values;

    for (auto& row : matrix_)
    {
        cost_t min = INF;
        for (auto& elem : row)
        {
            if (elem < min) min = elem;
        }
        if (min < INF - 100000) min_values.push_back(min);
        else min_values.push_back(0);
    }

    return min_values;
}

/**
 * Reduce rows so that in each row at least one zero value is present.
 * @return Sum of values reduced in rows.
 */
cost_t CostMatrix::reduce_rows() {
    std::vector<cost_t> min_values = get_min_values_in_rows();

    for (size_t i = 0; i < matrix_.size(); ++i)
    {
        for (size_t j = 0; j < matrix_.size(); ++j)
        {
            if (matrix_[i][j] != INF)
                matrix_[i][j] = matrix_[i][j] - min_values[i];
        }
    }

    cost_t sum_of_reduced = 0;
    if (!min_values.empty())
    {
        sum_of_reduced = std::accumulate(min_values.begin(), min_values.end(), 0);
    }

    return sum_of_reduced;
}

/**
 * Get minimum values from each column and returns them.
 * @return Vector of minimum values in columns.
 */
std::vector<cost_t> CostMatrix::get_min_values_in_cols() const {
    std::vector<cost_t> min_values(matrix_.size(), 0);

    for (size_t i = 0; i < matrix_.size(); ++i){
        cost_t min = INF;
        for (const auto & j : matrix_){
            if(j[i] < min)
                min = j[i];
        }
        if(min != INF)
        {
            if (min < INF - 100000) min_values[i] = min; // if (min < INF - 10000) zabezpieczenie przed wartościami troche mniejszymi od INF
        }
    }

    return min_values;
}

/**
 * Reduces rows so that in each column at least one zero value is present.
 * @return Sum of values reduced in columns.
 */
cost_t CostMatrix::reduce_cols() {
    std::vector<cost_t> min_values = get_min_values_in_cols();

    for (size_t i = 0; i < matrix_.size(); ++i)
    {
        for (size_t j = 0; j < matrix_.size(); ++j)
        {
            if (matrix_[j][i] != INF)
                matrix_[j][i] = matrix_[j][i] - min_values[i];
        }
    }

    cost_t sum_of_reduced = 0;
    if (!min_values.empty())
    {
        sum_of_reduced = std::accumulate(min_values.begin(), min_values.end(), 0);
    }

    return sum_of_reduced;
}

/**
 * Get the cost of not visiting the vertex_t (@see: get_new_vertex())
 * @param row
 * @param col
 * @return The sum of minimal values in row and col, excluding the intersection value.
 */
cost_t CostMatrix::get_vertex_cost(std::size_t row, std::size_t col) const {
    cost_t cost=0;

    int min_in_col = INT_MAX;
    for(size_t i = 0; i < matrix_.size(); ++i)
    {
        if ((i != col) and (matrix_[row][i] < min_in_col))
        {
            min_in_col = matrix_[row][i];
        }
    }

    int min_in_row = INT_MAX;
    for(size_t i = 0; i < matrix_.size(); ++i)
    {
        if ((i != row) && (matrix_[i][col] < min_in_row))
        {
            min_in_row = matrix_[i][col];
        }
    }

    cost = min_in_col + min_in_row;
    return cost;

}

/* PART 2 */

/**
 * Choose next vertex to visit:
 * - Look for vertex_t (pair row and column) with value 0 in the current cost matrix.
 * - Get the vertex_t cost (calls get_vertex_cost()).
 * - Choose the vertex_t with maximum cost and returns it.
 * @param
 * @return The coordinates of the next vertex.
 */
NewVertex StageState::choose_new_vertex() {
    std::vector<vertex_t> vertex_with_0;
    cost_t max_cost = 0;

    for (size_t i = 0; i < matrix_.size(); ++i)
    {
        for (size_t j = 0; j < matrix_.size(); ++j)
        {
            if (matrix_[i][j] == 0)
            {
                vertex_with_0.emplace_back(vertex_t(i, j));
            }
        }
    }

    for (auto& vertex : vertex_with_0)
    {
        if (matrix_.get_vertex_cost(vertex.row, vertex.col) > max_cost)
        {
            max_cost = matrix_.get_vertex_cost(vertex.row, vertex.col);
        }
    }

    vertex_t max_vertex;

    for (auto& vertex : vertex_with_0)
    {
        if (matrix_.get_vertex_cost(vertex.row, vertex.col) == max_cost)
        {
            max_vertex = vertex_t(vertex.row, vertex.col);
            break;
        }
    }


    return NewVertex(max_vertex, max_cost);
}


/**
 * Update the cost matrix with the new vertex.
 * @param new_vertex
 */
void StageState::update_cost_matrix(vertex_t new_vertex) {
    size_t inf_row = new_vertex.row;
    size_t inf_col = new_vertex.col;

// usunięcie i-tego wiersza i j-tej kolumny
    for (size_t i = 0; i < matrix_.size(); ++i)
    {
        for (size_t j = 0; j < matrix_.size(); ++j)
        {
            if (i == inf_row || j == inf_col)
            {
                matrix_[i][j] = INF;
            }
        }
    }


    matrix_[inf_col][inf_row] = INF; // zabronienie przejscia (jesli mamy 1,3 to zabraniamy 3,1)


}

/**
 * Reduce the cost matrix.
 * @return The sum of reduced values.
 */
cost_t StageState::reduce_cost_matrix() {
    cost_t reduced_val_in_rows = matrix_.reduce_rows();
    cost_t reduced_val_in_cols = matrix_.reduce_cols();


    return reduced_val_in_rows + reduced_val_in_cols;
}

/**
 * Given the optimal path, return the optimal cost.
 * @param optimal_path
 * @param m
 * @return Cost of the path.
 */
cost_t get_optimal_cost(const path_t& optimal_path, const cost_matrix_t& m) {
    cost_t cost = 0;

    for (std::size_t idx = 1; idx < optimal_path.size(); ++idx) {
        cost += m[optimal_path[idx - 1] - 1][optimal_path[idx] - 1];
    }

    // Add the cost of returning from the last city to the initial one.
    cost += m[optimal_path[optimal_path.size() - 1] - 1][optimal_path[0] - 1];

    return cost;
}

/**
 * Create the right branch matrix with the chosen vertex forbidden and the new lower bound.
 * @param m
 * @param v
 * @param lb
 * @return New branch.
 */
StageState create_right_branch_matrix(cost_matrix_t m, vertex_t v, cost_t lb) {
    CostMatrix cm(m);
    cm[v.row][v.col] = INF;
    return StageState(cm, {}, lb);
}

/**
 * Retain only optimal ones (from all possible ones).
 * @param solutions
 * @return Vector of optimal solutions.
 */
tsp_solutions_t filter_solutions(tsp_solutions_t solutions) {
    cost_t optimal_cost = INF;
    for (const auto& s : solutions) {
        optimal_cost = (s.lower_bound < optimal_cost) ? s.lower_bound : optimal_cost;
    }

    tsp_solutions_t optimal_solutions;
    std::copy_if(solutions.begin(), solutions.end(),
                 std::back_inserter(optimal_solutions),
                 [&optimal_cost](const tsp_solution_t& s) { return s.lower_bound == optimal_cost; }
    );

    return optimal_solutions;
}


/**
 * Solve the TSP.
 * @param cm The cost matrix.
 * @return A list of optimal solutions.
 */
tsp_solutions_t solve_tsp(const cost_matrix_t& cm) {

    StageState left_branch(cm);
//    CostMatrix mat = left_branch.get_matrix();

    // The branch & bound tree.
    std::stack<StageState> tree_lifo;

    // The number of levels determines the number of steps before obtaining
    // a 2x2 matrix.
    std::size_t n_levels = cm.size() - 2;

    tree_lifo.push(left_branch);   // Use the first cost matrix as the root.

    cost_t best_lb = INF;
    tsp_solutions_t solutions;

    while (!tree_lifo.empty()) {

        left_branch = tree_lifo.top();
//        std::cout << tree_lifo.size() << std::endl;
        tree_lifo.pop();
//        std::cout << tree_lifo.size() << std::endl;

        while (left_branch.get_level() != n_levels && left_branch.get_lower_bound() <= best_lb) {
            // Repeat until a 2x2 matrix is obtained or the lower bound is too high...

            if (left_branch.get_level() == 0) {
                left_branch.reset_lower_bound();
            }

            // 1. Reduce the matrix in rows and columns.
            cost_t new_cost = 0; // @TODO (KROK 1)
            new_cost = left_branch.reduce_cost_matrix();


            // 2. Update the lower bound and check the break condition.
            left_branch.update_lower_bound(new_cost);
            if (left_branch.get_lower_bound() > best_lb) {
                break;
            }

            // 3. Get new vertex and the cost of not choosing it.
            NewVertex new_vertex = NewVertex(); // @TODO (KROK 2)
            new_vertex = left_branch.choose_new_vertex();


            // 4. @TODO Update the path - use append_to_path method.
            left_branch.append_to_path(new_vertex.coordinates);

            // 5. @TODO (KROK 3) Update the cost matrix of the left branch.
            left_branch.update_cost_matrix(new_vertex.coordinates);

            // 6. Update the right branch and push it to the LIFO.
            cost_t new_lower_bound = left_branch.get_lower_bound() + new_vertex.cost;
            tree_lifo.push(create_right_branch_matrix(cm, new_vertex.coordinates,
                                                      new_lower_bound));
        }

        if (left_branch.get_lower_bound() <= best_lb) {
            // If the new solution is at least as good as the previous one,
            // save its lower bound and its path.
            best_lb = left_branch.get_lower_bound();
            path_t new_path = left_branch.get_path();
            solutions.push_back({get_optimal_cost(new_path, cm), new_path});
        }
    }

    return filter_solutions(solutions); // Filter solutions to find only optimal ones.
}
