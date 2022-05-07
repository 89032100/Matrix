#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;
class dimension_exception : public exception {
public:
	const char* what() {
		return "dimension_exception!";
	}
};
class singular_exception : public exception {
public:
	const char* what() {
		return "singular_exception!";
	}
};
class Matrix {
public:
	float** matrix;
	float** answer;
	int h, w;
	Matrix() {
		matrix = 0;
		answer = 0;
		h = w = 0;
	}
	Matrix(int h, int w) {
		if (h != w) throw dimension_exception();
		this->h = h;
		this->w = w;
		matrix = new float* [h];
		for (int i = 0; i < h; i++) {
			matrix[i] = new float[w];
		}
		answer = new float* [h];
		for (int i = 0; i < h; i++) {
			answer[i] = new float[w * 2];
		}
	}
	Matrix& inverse() {
		int i = 0;
		int j = 0;
		while (i < h && j < w) {
			int maxi = i;
			for (int k = i + 1; k < h; k++) {
				if (abs(answer[k][j]) > abs(answer[maxi][j])) maxi = k;
			}
			if (answer[maxi][j] != 0) {
				swap(answer[maxi], answer[i]);
				float temp = answer[i][j];
				for (int k = 0; k < w * 2; k++) {
					answer[i][k] /= temp;
				}
				for (int k = i + 1; k < h; k++) {
					float temp = answer[k][j];
					for (int l = j; l < w * 2; l++) {
						answer[k][l] -= answer[i][l] * temp;
						check_zero(answer[k][l]);
					}
				}
				i++;
			}
			j++;
		}
		for (int k = h - 1; k > 0; k--) {
			if (answer[k][k] == 0) throw singular_exception();
			for (int i = 0; i < k; i++) {
				float temp = answer[i][k];
				for (int j = 0; j < w * 2; j++) {
					answer[i][j] -= answer[k][j] * temp;
					check_zero(answer[i][j]);
				}
			}
		}
		return *this;
	}
	void swap(float*& m1, float*& m2) {
		float* temp = m1;
		m1 = m2;
		m2 = temp;
	}
	void check_zero(float& f) {
		if (f<1e-5 && f>-(1e-5)) f = 0;
	}
	friend istream& operator>>(istream& is, Matrix& m) {
		for (int i = 0; i < m.h; i++) {
			for (int j = 0; j < m.w; j++) {
				is >> m.matrix[i][j];
			}
		}
		for (int i = 0; i < m.h; i++) {
			for (int j = 0; j < m.w; j++) {
				m.answer[i][j] = m.matrix[i][j];
			}
			for (int j = m.w; j < m.w * 2; j++) {
				m.answer[i][j] = (j - i == m.h ? 1.0f : 0.0f);
			}
		}
		return is;
	}
	friend ostream& operator<<(ostream& os, Matrix& m) {
		for (int i = 0; i < m.h; i++) {
			for (int j = 0; j < m.w; j++) {
				os << setw(7)<< setprecision(3) << m.answer[i][j] << " ";
			}
			os << "|";
			for (int j = m.w; j < m.w * 2; j++) {
				os << setw(7) << setprecision(3) << m.answer[i][j] << " ";
			}
			os << endl;
		}
		return os;
	}
	~Matrix() {
		delete[] matrix;
		delete[] answer;
	}
};
int main() {
	int M, N;
	cin >> M >> N;
	try {
		Matrix m(M, N);
		cin >> m;
		cout << m << endl;
		cout << m.inverse() << endl;
	}
	catch (dimension_exception& e) {
		cout << e.what() << endl;
	}
	catch (singular_exception& e) {
		cout << e.what() << endl;
	}
	catch (bad_alloc& e) {
		cout << e.what() << endl;
	}
}
