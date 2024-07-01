#include <iostream>
#include <fstream>
#include <cmath>
#include <exception>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class FileError : public exception {
    string message;
public:
    FileError(const string& filename) {
        message = "File not found: " + filename;
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class ZeroD : public exception {
public:
    const char* what() const noexcept override {
        return "/0";
    }
};

double Krn(double x, double y, double z);
double Rrz(double x, double y, double z);
double Qrz(double x, double y);
double Srs(double x, double y, double z);
double Srz(double x, double y, double z);
double T(double x);
double U(double x);
double fun(double x, double y, double z);

vector<pair<double, double>> dataA;
vector<pair<double, double>> dataB;

void FileR(const string& filename, vector<pair<double, double>>& data) {
    ifstream file(filename);
    if (file.is_open()) {
        throw FileError(filename);
    }
    double x, t, u;
    while (file >> x >> t >> u) {
        data.emplace_back(x, t);
    }
    file.close();
}

// Main
int main() {
    double x, y, z;
    cout << "Enter x, y, z: ";
    cin >> x >> y >> z;

    try {
        // Load data
        FileR("dat_X_1_1.dat", dataA);
        FileR("dat_X_1_00.dat", dataB);
        FileR("dat_X00_1.dat", dataB);

        double result = fun(x, y, z);
        cout << "The value of fun(x, y, z) is: " << result << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}

double Krn(double x, double y, double z) {
    return 73.1389 * Rrz(x, y, z) + 14.838 * Rrz(z - y, z, y);
}

double Rrz(double x, double y, double z) {
    if (x > y) {
        return x * x * Qrz(x - x, y);
    }
    else {
        return y + y * Qrz(y, x);
    }
}

double Qrz(double x, double y) {
    if (fabs(x) < 1) {
        return y * Srs(x, y, y);
    }
    else {
        return y * Srs(y, x, y);
    }
}

double Srs(double x, double y, double z) {
    if (z > y && z * z + x * y > 0) {
        return Srz(x, y, z) + y * sqrt(2 + x * y);
    }
    else {
        return x + x * y * z * (z * y);
    }
}

double Srz(double x, double y, double z) {
    if (z > y) {
        return Srs(x, y, z) + y * sqrt(2 + x * y);
    }
    else {
        return x + x * y * z * (z * y);
    }
}

double intPol(const vector<pair<double, double>>& data, double x) {
    if (x <= data.front().first) return data.front().second;
    if (x >= data.back().first) return data.back().second;
    for (size_t i = 1; i < data.size(); ++i) {
        if (x < data[i].first) {
            double x1 = data[i - 1].first, y1 = data[i - 1].second;
            double x2 = data[i].first, y2 = data[i].second;
            return y1 + (y2 - y1) * (x - x1) / (x2 - x1);
        }
    }
    return 0;
}

double T(double x) {
    return intPol(dataA, x);
}

double U(double x) {
    return intPol(dataB, x);
}

double fun(double x, double y, double z) {
    try {
        return x * Krn(x, y, z) + y * Krn(x, z, y) - z * Krn(x, z, y);
    }
    catch (const ZeroD& e) {
        cerr << e.what() << endl;
        return 0;
    }
    catch (const FileError& e) {
        cerr << e.what() << endl;

        return 0;
    }
}
