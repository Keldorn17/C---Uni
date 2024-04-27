#include <iostream>
#include <ctime>

int main() {
    // ** Get Matrix Size **
    int size = 1001;
    while (!(size < 1001 && size > 0)) {
        printf("Give me a number that should be the size of the matrix (1 - 1000): ");
        scanf_s("%d", &size);
    }

    // ** Create Matrix **
    double** A = new double* [size];
    double** AInverse = new double* [size];
    double** Identity = new double* [size];
    double** L = new double* [size];
    double** U = new double* [size];
    double** B = new double* [size];

    for (int i = 0; i < size; ++i) {
        A[i] = new double[size]();
        AInverse[i] = new double[size]();
        Identity[i] = new double[size]();
        L[i] = new double[size]();
        U[i] = new double[size]();
        B[i] = new double[size]();
    }

    // ** Fill [Identity] **
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            if (i == j) Identity[i][j] = 1;
            else Identity[i][j] = 0;
        }

    // ** Fill [A] with random numbers (0 - 100) **
    setlocale(LC_ALL, "Hun");
    srand((unsigned)time(nullptr));
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            A[i][j] = rand() % 101;
        }

    // ** Print [A] and [Identity] **
    printf("[A]: \n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("\t%f", A[i][j]);
        }
        printf("\n");
    }
    printf("[Identity]: \n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("\t%f", Identity[i][j]);
        }
        printf("\n");
    }

    // ** Solve (A = L * U) for L and U **
    for (int i = 0; i < size; i++) {
        // Calculate [U] matrix
        for (int k = i; k < size; k++) {
            double sum = 0;
            for (int j = 0; j < i; j++)
                sum += L[i][j] * U[j][k];
            U[i][k] = A[i][k] - sum;
        }
        // Calculate [L] matrix
        for (int k = i; k < size; k++) {
            if (i == k)
                L[i][i] = 1;
            else {
                double sum = 0;
                for (int j = 0; j < i; j++)
                    sum += L[k][j] * U[j][i];
                L[k][i] = (A[k][i] - sum) / U[i][i];
            }
        }
    }

    // ** Print [L] and [U] **
    printf("[L]: \n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("\t%f", L[i][j]);
        }
        printf("\n");
    }

    printf("[U]: \n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("\t%f", U[i][j]);
        }
        printf("\n");
    }

    // ** Solve for [B] using forward substitution (L * B = Identity) **
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            double sum = Identity[i][j];
            for (int k = 0; k < i; k++) {
                sum -= L[i][k] * B[k][j];
            }
            B[i][j] = sum / L[i][i];

        }
    }

    // ** Solve for [AInverse] using backward substitution (U * X = B) and (X = AInverse) **
    for (int i = size - 1; i >= 0; i--) {
        for (int j = 0; j < size; j++) {
            double sum = B[i][j];
            for (int k = i + 1; k < size; k++)
                sum -= U[i][k] * AInverse[k][j];
            AInverse[i][j] = sum / U[i][i];
        }
    }

    // ** Print [B] and [AInverse] **
    printf("[B]: \n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("\t%f", B[i][j]);
        }
        printf("\n");
    }

    printf("[A Inverse]: \n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("\t%f", AInverse[i][j]);
        }
        printf("\n");
    }

    // ** Free alocated memory **
    for (int i = 0; i < size; ++i) {
        delete[] A[i];
        delete[] AInverse[i];
        delete[] Identity[i];
        delete[] L[i];
        delete[] U[i];
        delete[] B[i];
    }
    delete[] A;
    delete[] AInverse;
    delete[] Identity;
    delete[] L;
    delete[] U;
    delete[] B;

    return 0;
}

