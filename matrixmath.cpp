#include "pch.h"
#include "stdafx.h"
#include <iostream>

int matrix[4][4] = { 0, 1, 1, 0, -1, 0, 0, -1, 1, 0, 0, -1, 0, 1, -1, 0 };
int decaylines[4] = { 887, 1279, 2089, 3167 };
int outDecays[4] = { 0.8, 0.8, 0.8, 0.8 };
double decay_lo = 1.5;
double decay_hi = 0.7;

/*
int determ(int a[MAX][MAX], int n) {
	int det = 0, p, h, k, i, j, temp[MAX][MAX];
	if (n == 1) {
		return a[0][0];
	}
	else if (n == 2) {
		det = (a[0][0] * a[1][1] - a[0][1] * a[1][0]);
		return det;
	}
	else {
		for (p = 0; p < n; p++) {
			h = 0;
			k = 0;
			for (i = 1; i < n; i++) {
				for (j = 0; j < n; j++) {
					if (j == p) {
						continue;
					}
					temp[h][k] = a[i][j];
					k++;
					if (k == n - 1) {
						h++;
						k = 0;
					}
				}
			}
			det = det + a[0][p] * pow(-1, p)*determ(temp, n - 1);
		}
		return det;
	}
}

// multiply two matricies
//	[r1, c1] = sizeof(matrix)
r1 = size(matrix);
c1 = size(matrix[1]);
for (i = 0; i < r1; ++i)
	for (j = 0; j < c2; ++j)
		for (k = 0; k < c1; ++k)
		{
			mult[i][j] += a[i][k] * b[k][j];
		}

*/

using namespace std;

int main() {

}
