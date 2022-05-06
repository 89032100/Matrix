#include <iostream>
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
			answer[i] = new float[w];
		}
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				answer[i][j] = (i == j ? 1 : 0);
			}
		}
	}
	void inverse() {
		int i = 0;
		int j = 0;
		while (i < h && j < w) {
			int maxi = i;
			for (int k = i + 1; k < h; k++) {
				if (abs(matrix[k][j]) > abs(matrix[maxi][j])) maxi = k;
			}
			if (matrix[maxi][j] != 0) {
				swap(matrix[maxi], matrix[i]);
				swap(answer[maxi], answer[i]);
				for (int k = 0; k < w; k++) {
					matrix[i][k] /= matrix[i][0];
					answer[i][k] /= answer[i][0];
				}
				for (int k = i + 1; k < h; k++) {
					for (int l = j; l < w; l++) {
						matrix[k][l] -= matrix[i][j] * matrix[k][l];
					}
				}
			}

		}
	}
	void swap(float*& m1,float*& m2) {
		float*& temp = m1;
		m1 = m2;
		m2 = temp;
	}
	friend iostream& operator>>(iostream& is, Matrix& m) {
		for (int i = 0; i < m.h; i++) {
			for (int j = 0; j < m.w; j++) {
				is >> m.matrix[i][j];
			}
		}
		return is;
	}
	friend ostream& operator<<(ostream& os, Matrix& m) {
		for (int i = 0; i < m.h; i++) {
			for (int j = 0; j < m.w; j++) {
				os << m.matrix[i][j];
			}
		}
		return os;
	}
	~Matrix() {
		for (int i = 0; i < h; i++) {
			delete[] matrix[i];
			delete[] answer[i];
		}
		delete[] matrix;
		delete[] answer;
	}
};
int main() {

}