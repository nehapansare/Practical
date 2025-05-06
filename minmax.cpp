//Implement min,max,sum and average operation using parallel reduction

#include <iostream>
#include <omp.h>
#include <vector>
#include <climits>

using namespace std;

int main() {
    vector<int> vec = {12, 45, 23, 67, 34, 89, 25};

    int minVal = INT_MAX;
    int maxVal = INT_MIN;
    long long sum = 0;
    double avg = 0.0;

    // Parallel computation using OpenMP reductions
    #pragma omp parallel for reduction(min:minVal) reduction(max:maxVal) reduction(+:sum)
    for (int i = 0; i < vec.size(); i++) {
        minVal = min(minVal, vec[i]);
        maxVal = max(maxVal, vec[i]);
        sum += vec[i];
    }

    avg = static_cast<double>(sum) / vec.size();

    // Output the results
    cout << "Min: " << minVal << endl;
    cout << "Max: " << maxVal << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << avg << endl;

    return 0;
}
