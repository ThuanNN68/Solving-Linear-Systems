#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<math.h>
#include <windows.h>

#define EPS 1e-9

typedef struct Frac {
    int num; // tử số
    int den; // mẫu số
} fraction;

// Khai báo nguyên mẫu hàm
fraction InputFraction();
fraction AddFraction(fraction a, fraction b);
fraction SubtractFraction(fraction a, fraction b);
fraction MultiplyFraction(fraction a, fraction b);
fraction DivideFraction(fraction a, fraction b);
fraction SimplifyFraction(fraction a);
void PrintFraction(fraction a);
fraction **CreateMatrix(int M, int N);
void InputMatrix(fraction **mat, int M, int N);
int GetFractionWidth(fraction a);
void PrintMatrix(fraction **mat, int M, int N);
void createAugmentedMatrix(fraction **A, fraction **B, fraction **AB, int m, int n);
void printAugmentedMatrix(fraction **aug, int M, int N);
void SwapRows(fraction **mat, int row1, int row2, int cols);
fraction **GaussForwardElimination(fraction **aug, int M, int N);
void GaussBackSubstitution(fraction **aug, int M, int N);
void GaussianElimination(fraction **mat, fraction **b, int M, int N);
fraction MatrixDeterminant(fraction **mat, int M, int N);
fraction FindPivot(fraction **mat, int M, int N, int *solved_row, int *solved_col, int *row_pivot, int *col_pivot);
fraction **GJForwardElimination(fraction **mat, fraction **b, int M, int N);
void GJBackSubstitution(fraction **aug, int M, int N);
void GaussJordanElimination(fraction **mat, fraction **b, int M, int N);
void FreeMatrix(fraction **mat, int N);

// Chương trình chính
int main() {
    int M, N;
    printf("Nhap so hang cua ma tran: ");
    scanf("%d", &M);
    printf("Nhap so cot cua ma tran: ");
    scanf("%d", &N);

    // Tạo ma trận A và B
    fraction **A = CreateMatrix(M, N);
    fraction **B = CreateMatrix(M, 1);

    // Nhập ma trận A và B
    InputMatrix(A, M, N);
    InputMatrix(B, M, 1);

    // In ma trận A và B
    printf("Ma tran A:\n");
    PrintMatrix(A, M, N);
    printf("Ma tran B:\n");
    PrintMatrix(B, M, 1);

    // Giải hệ phương trình
    printf("\nGiai he phuong trinh theo phuong phap Gauss:\n");
    GaussianElimination(A, B, M, N);
    printf("\nGiai he phuong trinh theo phuong phap Gauss-Jordan:\n");
    GaussJordanElimination(A, B, M, N);

    // Tính định thức của ma trận A
    fraction det = MatrixDeterminant(A, M, N);
    printf("Dinh thuc cua ma tran A la: ");
    PrintFraction(det);
    printf("\n");

    // Giải phóng bộ nhớ
    FreeMatrix(A, M);
    FreeMatrix(B, M);

    return 0;
}

// Hàm rút gọn phân số
fraction SimplifyFraction(fraction a) {
    int gcdps = a.num;
    int temp = a.den;
    
    // Tính GCD bằng thuật toán Euclidean
    while (temp != 0) {
        int remainder = gcdps % temp;
        gcdps = temp;
        temp = remainder;
    }
    
    // Rút gọn phân số
    if (gcdps != 0) {
        a.num /= gcdps;
        a.den /= gcdps;
    }
    
    // Xử lý dấu
    if (a.den < 0) {
        a.num = -a.num;
        a.den = -a.den;
    }
    
    return a;
}

// Hàm nhập phân số
fraction InputFraction() {
    fraction f;
    printf("Nhap tu so: ");
    scanf("%d", &f.num);
    printf("Nhap mau so: ");
    scanf("%d", &f.den);
    if (f.den == 0) {
        printf("Mau so khong hop le. Vui long nhap lai.\n");
        getch();
        return InputFraction();
    }
    // Rút gọn phân số
    f = SimplifyFraction(f);
    return f;
}

// Hàm cộng 2 phân số
fraction AddFraction(fraction a, fraction b) {
    fraction c;
    c.num = a.num * b.den + b.num * a.den;
    c.den = a.den * b.den;
    c = SimplifyFraction(c);
    return c;
}

// Hàm trừ 2 phân số
fraction SubtractFraction(fraction a, fraction b) {
    fraction c;
    c.num = a.num * b.den - b.num * a.den;
    c.den = a.den * b.den;
    c = SimplifyFraction(c);
    return c;
}

// Hàm nhân 2 phân số
fraction MultiplyFraction(fraction a, fraction b) {
    fraction c;
    c.num = a.num * b.num;
    c.den = a.den * b.den;
    c = SimplifyFraction(c);
    return c;
}

// Tàm chia 2 phân số
fraction DivideFraction(fraction a, fraction b) {
    fraction c;
    c.num = a.num * b.den;
    c.den = a.den * b.num;
    c = SimplifyFraction(c);
    return c;
}

// Hàm in phân số
void PrintFraction(fraction a) {
    a = SimplifyFraction(a);
    if (a.den == 1) printf("%d", a.num);
    else if (a.num == 0) printf("0");
    else printf("%d/%d", a.num, a.den);
}

// Hàm tạo ma trận
fraction **CreateMatrix(int M, int N) {
    fraction **mat = (fraction **)malloc(M * sizeof(fraction *));
    for (int i = 0; i < M; i++) {
        mat[i] = (fraction *)malloc(N * sizeof(fraction));
    }
    return mat;
}

// Hàm nhập ma trận 
void InputMatrix(fraction **mat, int M, int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("Nhap phan tu thu %d, %d: ", i + 1, j + 1);
            mat[i][j] = InputFraction();
        }
    }
}

// Hàm tính độ rộng của một phân số khi in
int GetFractionWidth(fraction a) {
    char buffer[50];
    if (a.den == 1) {
        sprintf(buffer, "%d", a.num);
    } else {
        sprintf(buffer, "%d/%d", a.num, a.den);
    }
    return strlen(buffer);
}

// Hàm in ma trận với căn chỉnh
void PrintMatrix(fraction **mat, int M, int N) {
    // Tìm độ rộng lớn nhất cho mỗi cột
    int *colWidths = (int *)malloc(N * sizeof(int));
    for (int j = 0; j < N; j++) {
        colWidths[j] = 0;
        for (int i = 0; i < M; i++) {
            int width = GetFractionWidth(mat[i][j]);
            if (width > colWidths[j]) {
                colWidths[j] = width;
            }
        }
    }

    // In ma trận với căn chỉnh
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            char buffer[50];
            if (mat[i][j].den == 1) {
                sprintf(buffer, "%d", mat[i][j].num);
            } else {
                sprintf(buffer, "%d/%d", mat[i][j].num, mat[i][j].den);
            }
            printf("%*s ", colWidths[j], buffer);
        }
        printf("\n");
    }
    free(colWidths);
}

// Hàm tạo ma trận mở rộng A|B
void createAugmentedMatrix(fraction **A, fraction **B, fraction **AB, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            AB[i][j] = A[i][j];
        }
        for (int j = n; j < n + 1; j++) {
            AB[i][j] = B[i][0];
        }
    }
}

// Hàm in ma trận mở rộng
void printAugmentedMatrix(fraction **aug, int M, int N) {
    int *colWidths = (int *)malloc((N + 1) * sizeof(int));
    char buffer[50];
    // Tìm độ rộng lớn nhất cho mỗi cột
    for (int j = 0; j < N + 1; j++) {
        colWidths[j] = 0;
        for (int i = 0; i < M; i++) {
            int width = GetFractionWidth(aug[i][j]);
            if (width > colWidths[j]) {
                colWidths[j] = width;
            }
        }
    }

    // In ma trận với căn chỉnh
    for (int i = 0; i < M; i++) {
        // In phần ma trận A
        for (int j = 0; j < N; j++) {
            if (aug[i][j].den == 1) {
                sprintf(buffer, "%d", aug[i][j].num);
            } else {
                sprintf(buffer, "%d/%d", aug[i][j].num, aug[i][j].den);
            }
            printf("%*s ", colWidths[j], buffer);
        }
        printf("| ");
        // In phần ma trận B
        for (int j = N; j < N + 1; j++) {
            if (aug[i][j].den == 1) {
                sprintf(buffer, "%d", aug[i][j].num);
            } else {
                sprintf(buffer, "%d/%d", aug[i][j].num, aug[i][j].den);
            }
            printf("%*s ", colWidths[j], buffer);
        }
        printf("\n");
    }
    free(colWidths);
}

// Hàm hoán đổi 2 hàng của ma trận mở rộng 
void SwapRows(fraction **mat, int row1, int row2, int cols) {
    // Biến để lưu phân số tạm thời
    fraction temp;
    for (int i = 0; i < cols; i++) {
        temp = mat[row1][i];
        mat[row1][i] = mat[row2][i];
        mat[row2][i] = temp;
    }
}

fraction **GaussForwardElimination(fraction **aug, int M, int N) {
    fraction f; // Biến để lưu phân số f
    for (int k = 0; k < M - 1; k++) {
        if (abs(aug[k][k].num) < EPS) {
            for (int i = k; i > 0; i--) {
                if (abs(aug[i][k].num) > EPS) {
                    SwapRows(aug, k, i, N);
                    printf("Swap rows %d and %d\n", k, i);
                    printAugmentedMatrix(aug, M, N);
                    break;
                }
            }
        }

        // Khử các phần tử dưới pivot
        for (int i = k + 1; i < M; i++) {
            f = DivideFraction(aug[i][k], aug[k][k]);
            for (int j = k; j < N + 1; j++) {
                aug[i][j] = SubtractFraction(aug[i][j], MultiplyFraction(aug[k][j], f));
            }
            printAugmentedMatrix(aug, M, N);
        }
    }
    return aug;
}

// Hàm thế ngược 
void GaussBackSubstitution(fraction **aug, int M, int N) {
    int count_mat = 0; // Số hàng khác 0 của ma trận A
    int count_aug = 0; // Số hàng khác 0 của ma trận mở rộng
    int zero_rows; // Biến để kiểm tra số hàng khác 0
    int non_zero_rows; // Biến để kiểm tra số hàng khác 0
    int independent_rows; // Số hàng độc lập của ma trận A
    int free_variables; // Số biến tự do của nghiệm
    fraction sum; // Tổng của các phần tử trong hàng i
    fraction *x; // Mảng lưu nghiệm
    
    for (int i = 0; i < M; i++) {
        int zero_rows = 1;
        for (int j = 0; j < N; j++) {
            if (aug[i][j].num != 0) {
                zero_rows = 0;  
                break;
            }
        }
        if (zero_rows == 0) {
            count_mat++;
        }
    }

    // Đếm số hàng khác không của ma trận mở rộng
    count_aug = 0;
    for (int i = 0; i < M; i++) {
        int zero_rows = 1;
        for (int j = 0; j < N; j++) {
            if (aug[i][j].num != 0) {
                zero_rows = 0;
                break;
            }
        }
        if (zero_rows == 0) {
            count_aug++;
        }
    }

    printf("\n=== KET QUA ===\n");
    if (count_mat < count_aug) {
        printf("He phuong trinh vo nghiem\n");
        return;
    }
    else if (count_mat == count_aug && count_mat == M) {
        printf("He phuong trinh co nghiem duy nhat:\n");
        // Tìm nghiệm của hệ phương trình
        x = (fraction *)malloc(N * sizeof(fraction));
        for (int i = M - 1; i >= 0; i--) {
            // Tính tổng của các phần tử trong hàng i
            sum = aug[i][N];
            for (int j = i + 1; j < N; j++) {
                sum = SubtractFraction(sum, MultiplyFraction(aug[i][j], x[j]));
            }
            x[i] = DivideFraction(sum, aug[i][i]);
        }

        // In nghiệm với định dạng đẹp hơn
        printf("\nNghiem cua he phuong trinh:\n");
        printf("------------------------\n");
        for (int i = 0; i < N; i++) {
            printf("x%d = ", i + 1);
            PrintFraction(x[i]);
            printf("\n");
        }
        printf("------------------------\n");
        free(x);
    }
    else {
        printf("He phuong trinh co vo so nghiem\n");
        // Tìm nghiệm của hệ phương trình
        non_zero_rows = 0;
        for (int i = 0; i < M; i++) {
            zero_rows = 1;
            for (int j = 0; j < N; j++) {
                if (aug[i][j].num != 0) {
                    zero_rows = 0;
                    break;
                }
            }
            if (zero_rows == 0) {
                non_zero_rows++;
            }
        }
        independent_rows = M - non_zero_rows;
        free_variables = N - independent_rows;
        if (free_variables > 0) {
            fraction **x = (fraction **)malloc(independent_rows * sizeof(fraction *));
            for (int k = 0; k < independent_rows; k++) {
                x[k] = (fraction *)malloc(free_variables * sizeof(fraction));
            }
            for (int i = independent_rows - 1; i >= 0; i--) {
                x[i][0] = aug[i][N];
                for (int j = 0; j < free_variables; j++) {
                    x[i][independent_rows - j] = DivideFraction(
                        MultiplyFraction((fraction){-1, 1}, aug[i][j + independent_rows]),
                        aug[i][i]
                    );
                }
            }
            printf("\nNghiem cua he phuong trinh:\n");
            printf("------------------------\n");
            for (int i = 0; i < independent_rows; i++) {
                printf("x%d = ", i + 1);
                for (int j = 0; j < free_variables; j++) {
                    if (j == 0) {
                        PrintFraction(x[i][0]);
                    } else {
                        printf(" + ");
                        PrintFraction(x[i][j]);
                        printf(" * t%d", j);
                    }
                }
                printf("\n");
            }
            printf("------------------------\n");
            printf("Trong do t1, t2, ... la cac tham so tu do\n");
            
            // Giải phóng bộ nhớ
            for (int i = 0; i < independent_rows; i++) {
                free(x[i]);
            }
            free(x);
        }
    }
}

void GaussianElimination(fraction **mat, fraction **b, int M, int N) {
    fraction **aug = (fraction **)malloc(M * sizeof(fraction *));
    for (int i = 0; i < M; i++) {
        aug[i] = (fraction *)malloc((N + 1) * sizeof(fraction));
    }
    
    // Tạo ma trận mở rộng
    createAugmentedMatrix(mat, b, aug, M, N);

    // Thực hiện khử Gauss
    aug = GaussForwardElimination(aug, M, N);

    // Thực hiện thế ngược
    GaussBackSubstitution(aug, M, N);
}

// Tính định thức của ma trận M x N theo thuật toán khử Gauss
fraction MatrixDeterminant(fraction **mat, int M, int N) {
    // Khởi tạo det = 1/1
    fraction det = {1, 1}; 
    // Tạo phân số 0 
    fraction zero = {0, 1};  
    if (M != N) {
        printf("Ma tran khong la ma tran vuong.\n");
        return zero;
    }
    for (int i = 0; i < M; i++) {
        det = MultiplyFraction(det, mat[i][i]);
    }
    return det;
}

// Hàm tìm phần tử khử trong phương pháp Gauss - Jordan
fraction FindPivot(fraction **mat, int M, int N, int *solved_row, int *solved_col, int *row_pivot, int *col_pivot) {
    // Khởi tạo biến max_val và max_row, max_col
    fraction max_val = mat[0][0];
    int max_row = -1, max_col = -1;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (solved_row[i] == 1 || solved_col[j] == 1) {
                continue;
            }
            if (abs(mat[i][j].num) > abs(max_val.num)) {
                max_val = mat[i][j];
                max_row = i;
                max_col = j;
                *row_pivot = i;
                *col_pivot = j;
            }
        }
    }
    // Kiểm tra nếu tìm thấy phần tử pivot
    if (max_row != -1 && max_col != -1) {
        printf("Pivot element at row %d, col %d: ", max_row, max_col);
        PrintFraction(max_val);
        printf("\n");
        solved_row[max_row] = 1;
        solved_col[max_col] = 1;
    }
    return max_val;
}

// Hàm thực hiện khử Gauss - Jordan
fraction **GJForwardElimination(fraction **mat, fraction **b, int M, int N) {
    // Khởi tạo các biến 
    fraction p;
    // Tạo biến để lưu hàng và cột pivot
    int row_pivot, col_pivot;
    // Tạo mảng đánh dấu hàng và cột đã giải
    int *solved_row = (int *)malloc(M * sizeof(int));
    int *solved_col = (int *)malloc(N * sizeof(int));

    // Tạo ma trận mở rộng 
    fraction **aug = (fraction **)malloc(M * sizeof(fraction *));
    for (int i = 0; i < M; i++) {
        aug[i] = (fraction *)malloc((N + 1) * sizeof(fraction));
    }   
    createAugmentedMatrix(mat, b, aug, M, N);

    for (int i = 0; i < M; i++) {
        solved_row[i] = 0;
    }
    for (int i = 0; i < N; i++) {
        solved_col[i] = 0;
    }
    
    // Thực hiện khử Gauss - Jordan
    for (int i = 0; i < N; i++) {
        fraction pivot = FindPivot(mat, M, N, solved_row, solved_col, &row_pivot, &col_pivot);
        for (int k = 0; k < M; k++) {
            // Chuẩn hóa hàng chứa pivot
            if (k == row_pivot) {
                mat[k][col_pivot] = DivideFraction(mat[k][col_pivot], pivot);
                aug[k][col_pivot] = DivideFraction(aug[k][col_pivot], pivot);
            }
            else {
                // Khử các phần tử trên cùng cột với pivot
                // Tính toán phân số p
                p = DivideFraction(mat[k][col_pivot], pivot);
                for (int j = 0; j < N; j++) {
                    mat[k][j] = SubtractFraction(mat[k][j], MultiplyFraction(p, mat[row_pivot][j]));
                    aug[k][j] = SubtractFraction(aug[k][j], MultiplyFraction(p, aug[row_pivot][j]));
                }
                aug[k][N] = SubtractFraction(aug[k][N], MultiplyFraction(p, aug[row_pivot][N]));
            }
        }
        printAugmentedMatrix(aug, M, N);
    }
    return aug;
}

void GJBackSubstitution(fraction **aug, int M, int N){
    // Khởi tạo các biến
    int independent_rows; // Số hàng độc lập của ma trận A
    int free_variables; // Số biến tự do của nghiệm
    int count_mat; // Số hàng khác 0 của ma trận A
    int count_aug; // Số hàng khác 0 của ma trận mở rộng
    int zero_rows; // Biến để kiểm tra số hàng khác 0

    // Kiểm tra nghiệm của hệ phương trình
    // Đếm số hàng khác không của ma trận con A sau biến đổi
    count_mat = 0;
    for (int i = 0; i < M; i++) {
        zero_rows = 1;
        for (int j = 0; j < N; j++) {
            if (aug[i][j].num != 0) {
                zero_rows = 0;  
                break;
            }
        }
        if (zero_rows == 0) {
            count_mat++;
        }
    }

    // Đếm số hàng khác không của ma trận mở rộng
    count_aug = 0;
    for (int i = 0; i < M; i++) {
        zero_rows = 1;
        for (int j = 0; j < N + 1; j++) {
            if (aug[i][j].num != 0) {
                zero_rows = 0;
                break;
            }
        }
        if (zero_rows == 0) {
            count_aug++;
        }
    }

    printf("\n=== KET QUA ===\n");
    if (count_mat < count_aug) {
        printf("He phuong trinh vo nghiem\n");
        return;
    }
    else if (count_mat == count_aug && count_mat == M) {
        printf("He phuong trinh co nghiem duy nhat:\n");
        // Tìm nghiệm của hệ phương trình
        fraction *x = (fraction *)malloc(N * sizeof(fraction));
        for (int i = 0; i < N; i++) {
            x[i] = aug[i][N];
        }
        printf("\nNghiem cua he phuong trinh:\n");
        printf("------------------------\n");
        for (int i = 0; i < N; i++) {
            printf("x%d = ", i + 1);
            PrintFraction(x[i]);
            printf("\n");
        }
        printf("------------------------\n");
        free(x);
    }
    else {
        printf("He phuong trinh co vo so nghiem\n");
        // Tìm nghiệm của hệ phương trình
        int non_zero_rows = 0;
        for (int i = 0; i < M; i++) {
            zero_rows = 1;
            for (int j = 0; j < N + 1; j++) {
                if (aug[i][j].num != 0) {
                    zero_rows = 0;
                    break;
                }
            }
            if (zero_rows == 0) {
                non_zero_rows++;
            }
        }
        independent_rows = M - non_zero_rows;
        free_variables = N - independent_rows;
        if (free_variables > 0) {
            fraction **x = (fraction **)malloc(independent_rows * sizeof(fraction *));
            for (int k = 0; k < independent_rows; k++) {
                x[k] = (fraction *)malloc(free_variables * sizeof(fraction));
            }
            for (int i = independent_rows - 1; i >= 0; i--) {
                x[i][0] = aug[i][N];
                for (int j = 0; j < free_variables; j++) {
                    x[i][independent_rows - j] = DivideFraction(
                        MultiplyFraction((fraction){-1, 1}, aug[i][j + independent_rows]),
                        aug[i][i]
                    );
                }
            }
            printf("\nNghiem cua he phuong trinh:\n");
            printf("------------------------\n");
            for (int i = 0; i < independent_rows; i++) {
                printf("x%d = ", i + 1);
                for (int j = 0; j < free_variables; j++) {
                    if (j == 0) {
                        PrintFraction(x[i][0]);
                    } else {
                        printf(" + ");
                        PrintFraction(x[i][j]);
                        printf(" * t%d", j);
                    }
                }
                printf("\n");
            }
            printf("------------------------\n");
            printf("Trong do t1, t2, ... la cac tham so tu do\n");
            
            // Giải phóng bộ nhớ
            for (int i = 0; i < independent_rows; i++) {
                free(x[i]);
            }
            free(x);
        }
    }
}

void GaussJordanElimination(fraction **mat, fraction **b, int M, int N) {
    fraction **aug = GJForwardElimination(mat, b, M, N);
    GJBackSubstitution(aug, M, N);
}

// Giải phóng bộ nhớ
void FreeMatrix(fraction **mat, int N) {
    for (int i = 0; i < N; i++) {
        free(mat[i]);
    }
    free(mat);
}

