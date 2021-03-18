#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
using namespace std;
using std::vector;
bool is_in(int row, int col, vector<int> rows, vector<int> cols) {
	int temp = rows.size();
	for (int i = 0; i < temp; ++i) {
		if (rows[i] == row && cols[i] == col)
			return true;
	}
	return false;
}
bool are_close(vector<double> a, vector<double> b, double threshold) {
	int size = a.size();
	double temp = 0;
	for (int i = 0; i < size; ++i)
		temp += fabs(a[i] - b[i]);
	temp /= size;
	return temp < threshold;
}
bool is_out(int row, int col, int n_row, int n_col) {
	return row < 0 || row >= n_row || col < 0 || col >= n_col;
}
vector<vector<int>> state_space(int n_row, int n_col, vector<int> obs_row, vector<int> obs_col) {
	vector<int> row_set{};
	vector<int> col_set{};
	for (int i = 0; i < n_row; ++i) {
		for (int j = 0; j < n_col; ++j) {
			if (!is_in(i, j, obs_row, obs_col)) {
				row_set.push_back(i);
				col_set.push_back(j);
			}
		}
	}
	return { row_set, col_set };
}
vector<int> f(int row, int col, int row_move, int col_move, int end_row, int end_col) {
	if (row == end_row && col == end_col)
		return { row, col };
	else
		return { row + row_move, col + col_move };
}
double rho(int row, int col, int end_row, int end_col) {
	if (row == end_row && col == end_col)
		return 0;
	else
		return -1;
}
double V(vector<int> states_row, vector<int> states_col, vector<double> values, int row, int col) {
	int i = 0;
	while (1) {
		if (states_row[i] == row && states_col[i] == col)
			return values[i];
		++i;
	}
}
double value_update(int index, int n_row, int n_col, int end_row, int end_col, vector<int> obs_row, vector<int> obs_col, double gamma, vector<int> states_row, vector<int> states_col, vector<double> values) {
	vector<double> set{};
	vector<int> new_state;
	int temp = states_row[index] + 1;
	if (!is_in(temp, states_col[index], obs_row, obs_col) && !is_out(temp, states_col[index], n_row, n_col)) {
		new_state = f(states_row[index], states_col[index], 1, 0, end_row, end_col);
		set.push_back(rho(states_row[index], states_col[index], end_row, end_col) + gamma * V(states_row, states_col, values, new_state[0], new_state[1]));
	}
	temp = states_row[index] - 1;
	if (!is_in(temp, states_col[index], obs_row, obs_col) && !is_out(temp, states_col[index], n_row, n_col)) {
		new_state = f(states_row[index], states_col[index], -1, 0, end_row, end_col);
		set.push_back(rho(states_row[index], states_col[index], end_row, end_col) + gamma * V(states_row, states_col, values, new_state[0], new_state[1]));
	}
	temp = states_col[index] + 1;
	if (!is_in(states_row[index], temp, obs_row, obs_col) && !is_out(states_row[index], temp, n_row, n_col)) {
		new_state = f(states_row[index], states_col[index], 0, 1, end_row, end_col);
		set.push_back(rho(states_row[index], states_col[index], end_row, end_col) + gamma * V(states_row, states_col, values, new_state[0], new_state[1]));
	}
	temp = states_col[index] - 1;
	if (!is_in(states_row[index], temp, obs_row, obs_col) && !is_out(states_row[index], temp, n_row, n_col)) {
		new_state = f(states_row[index], states_col[index], 0, -1, end_row, end_col);
		set.push_back(rho(states_row[index], states_col[index], end_row, end_col) + gamma * V(states_row, states_col, values, new_state[0], new_state[1]));
	}
	return *max_element(set.begin(), set.end());
}
vector<double> V_iteration(double gamma, int n_row, int n_col, int end_row, int end_col, vector<int> obs_row, vector<int> obs_col, double threshold, vector<double> initial_values) {
	vector<vector<int>> states = state_space(n_row, n_col, obs_row, obs_col);
	vector<double> new_values = initial_values;
	int states_number = initial_values.size();
	while (1) {
		for (int i = 0; i < states_number; ++i)
			new_values[i] = value_update(i, n_row, n_col, end_row, end_col, obs_row, obs_col, gamma, states[0], states[1], initial_values);
		if (are_close(initial_values, new_values, threshold))
			break;
		initial_values = new_values;
	}
	return new_values;
}
int main(void) {
	int n_row = 6, n_col = 6, end_row = 5, end_col = 4;
	vector<int> obs_row{ 1, 2, 3, 3, 3, 3 };
	vector<int> obs_col{ 4, 4, 1, 2, 3, 4 };
	int states_number = n_row * n_col - obs_row.size();
	vector<double> initial_values{};
	for (int i = 0; i < states_number; ++i)
		initial_values.push_back(0);
	vector<double> new_values;
	new_values = V_iteration(0.1, n_row, n_col, end_row, end_col, obs_row, obs_col, 0.005, initial_values);
	cout << "gamma = 0.1" << endl;
	new_values = V_iteration(0.2, n_row, n_col, end_row, end_col, obs_row, obs_col, 0.005, new_values);
	cout << "gamma = 0.2" << endl;
	new_values = V_iteration(0.3, n_row, n_col, end_row, end_col, obs_row, obs_col, 0.005, new_values);
	cout << "gamma = 0.3" << endl;
	new_values = V_iteration(0.4, n_row, n_col, end_row, end_col, obs_row, obs_col, 0.005, new_values);
	cout << "gamma = 0.4" << endl;
	new_values = V_iteration(0.5, n_row, n_col, end_row, end_col, obs_row, obs_col, 0.005, new_values);
	cout << "gamma = 0.5" << endl;
	new_values = V_iteration(0.6, n_row, n_col, end_row, end_col, obs_row, obs_col, 0.005, new_values);
	cout << "gamma = 0.6" << endl;
	new_values = V_iteration(0.7, n_row, n_col, end_row, end_col, obs_row, obs_col, 0.005, new_values);
	cout << "gamma = 0.7" << endl;
	new_values = V_iteration(0.8, n_row, n_col, end_row, end_col, obs_row, obs_col, 0.005, new_values);
	cout << "gamma = 0.8" << endl;
	new_values = V_iteration(0.9, n_row, n_col, end_row, end_col, obs_row, obs_col, 0.005, new_values);
	cout << "gamma = 0.9" << endl;
	new_values = V_iteration(0.99, n_row, n_col, end_row, end_col, obs_row, obs_col, 0.005, new_values);
	cout << "gamma = 0.99" << endl;
	for (int i = 0; i < states_number; ++i)
		cout << new_values[i] << endl;
	return 0;
}