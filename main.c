#include <stdio.h>

void backtracking(int m[][5], int s, int f, FILE *fout){
    fprintf(fout, "no matrix with sum == %d and first == %d found", s, f);
}



int main() {
    FILE *fin, *fout;
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");
    int sum, first;
    fscanf(fin, "%d\n%d", &sum, &first);
    int matrix[5][5];
    matrix[0][0] = first;
    backtracking(matrix, sum, first, fout);
    return 0;
}
