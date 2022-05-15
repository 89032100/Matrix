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
	Matrix* answer;
	float** matrix;
	int h, w;
	Matrix() {
		matrix = 0;
		answer = 0;
		h = w = 0;
	}//初始化
	Matrix(int h, int w) {
		if (h != w) throw dimension_exception();
		this->h = h;
		this->w = w;
		matrix = new float* [h];
		for (int i = 0; i < h; i++) {
			matrix[i] = new float[w];
		}//動態宣告
		answer = new Matrix();
		answer->h = h;
		answer->w = w * 2;
		answer->matrix = new float* [h];
		for (int i = 0; i < h; i++) {
			answer->matrix[i] = new float[w * 2];
		}//宣告儲存反矩陣的Matrix物件
	}
	Matrix& inverse() {
		int i = 0;
		int j = 0;
		while (i < h && j < w) {
			int maxi = i;
			for (int k = i + 1; k < h; k++) {
				if (abs(answer->matrix[k][j]) > abs(answer->matrix[maxi][j])) maxi = k;
			}
			if (answer->matrix[maxi][j] != 0) {
				swap(answer->matrix[maxi], answer->matrix[i]);
				float temp = answer->matrix[i][j];
				for (int k = 0; k < w * 2; k++) {
					answer->matrix[i][k] /= temp;
					check_zero(answer->matrix[i][k]);
				}
				for (int k = i + 1; k < h; k++) {
					float temp = answer->matrix[k][j];
					for (int l = j; l < w * 2; l++) {
						answer->matrix[k][l] -= answer->matrix[i][l] * temp;
						check_zero(answer->matrix[k][l]);
					}
				}
				i++;
			}
			j++;
		}
		for (int k = h - 1; k > 0; k--) {
			if (answer->matrix[k][k] == 0) throw singular_exception();
			for (int i = 0; i < k; i++) {
				float temp = answer->matrix[i][k];
				for (int j = 0; j < w * 2; j++) {
					answer->matrix[i][j] -= answer->matrix[k][j] * temp;
					check_zero(answer->matrix[i][j]);
				}
			}
		}
		return *answer;
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
				m.answer->matrix[i][j] = m.matrix[i][j];
			}
			for (int j = m.w; j < m.w * 2; j++) {
				m.answer->matrix[i][j] = (j - i == m.h ? 1.0f : 0.0f);
			}
		}
		return is;
	}
	friend ostream& operator<<(ostream& os, Matrix& m) {
		for (int i = 0; i < m.h; i++) {
			for (int j = 0; j < m.w; j++) {
				os << setw(7) << setprecision(3) << m.matrix[i][j] << " ";
			}
			os << endl;
		}
		return os;
	}
	~Matrix() {
		delete answer;

		for (int i = 0; i < h; i++) {
			delete[] matrix[i];
		}
		delete[] matrix;
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
		cout << m << endl;
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