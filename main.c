#include <stdio.h>
#include <math.h>
#include <time.h>

int prime_ends[10000] = { 0 }; // в этом массиве будем отмечать всевозможные комбинации цифр которыми может заканчиваться простое число (для проверки диагоналей)
int cache[100000] = { 0 }; // в этом массиве будем отмечать наличие простого числа и всевозможные комбинации цифр с которых может начинаться нужное простое число (для быстрой проверки наличия)
int matrix_count = 0; // количество найденных матриц

// вывод массива пяти чисел в виде матрицы
void matrix_output(int m[5], FILE *fout) {
    matrix_count += 1;
    for (int i = 0; i < 5; i++){
        fprintf(fout, "%d ", m[i] / 10000);
        fprintf(fout, "%d ", (m[i] / 1000) % 10);
        fprintf(fout, "%d ", (m[i] / 100) % 10);
        fprintf(fout, "%d ", (m[i] / 10) % 10);
        fprintf(fout, "%d ", m[i] % 10);
        fprintf(fout, "\n");
    }
    fprintf(fout, "\n");
}

// проверка на простое число
int is_prime(int n) {
    if (n == 2) return 1;
    for (int div = 2; div <= sqrt(n) + 1; div++) {
        if (n % div == 0) return 0;
    }
    return 1;
}

// сумма цифр числа n
int find_sum_of_digits(int n) {
    return n % 10 + (n / 10) % 10 + (n / 100) % 10 + (n / 1000) % 10 + n / 10000;
}

// проверка на наличие чётных цифр в числе (последняя строка матрицы не должна содержать четных цифр)
int if_even_in_prime(int n) {
    if (n % 10 % 2 == 0) return 1;
    if ((n / 10) % 10 % 2 == 0) return 1;
    if ((n / 100) % 10 % 2 == 0) return 1;
    if ((n / 1000) % 10 % 2 == 0) return 1;
    if ((n / 10000) % 10 % 2 == 0) return 1;
    return 0;
}

// ищем пятизначные простые числа с нужной суммой и отмечаем их существование в отдельном массиве, чтобы потом проверять наличие за O(1)
void find_suitable_primes(int s[10000], int sum) {
    int ind = 0;
    for (int n = 10001; n <= 99999; n += 2) {
        if (is_prime(n) && find_sum_of_digits(n) == sum) {
            s[ind++] = n;
            cache[n] = 1;
            cache[n / 10] = 1;
            cache[n / 100] = 1;
            cache[n / 1000] = 1;
            cache[n / 10000] = 1;
            prime_ends[n % 10] = 1;
            prime_ends[n % 100] = 1;
            prime_ends[n % 1000] = 1;
            prime_ends[n % 10000] = 1;
        }
    }
}

// проверяем сумму цифр чисел по горизонтали и по вертикали, если матрица недостроена - простая оценка: попадает ли сумма в интервал между минимально и максимально возможными значениями
int check_sums(int m[5], int sum, int line) {

    if (line == 5) {
        // проверим вертикали
        if (m[0] % 10 + m[1] % 10 + m[2] % 10 + m[3] % 10 + m[4] % 10 != sum) return 0;
        if ((m[0] / 10) % 10 + (m[1] / 10) % 10 + (m[2] / 10) % 10 + (m[3] / 10) % 10 + (m[4] / 10) % 10 != sum) return 0;
        if ((m[0] / 100) % 10 + (m[1] / 100) % 10 + (m[2] / 100) % 10 + (m[3] / 100) % 10 + (m[4] / 100) % 10 != sum) return 0;
        if ((m[0] / 1000) % 10 + (m[1] / 1000) % 10 + (m[2] / 1000) % 10 + (m[3] / 1000) % 10 + (m[4] / 1000) % 10 != sum) return 0;
        if ((m[0] / 10000) % 10 + (m[1] / 10000) % 10 + (m[2] / 10000) % 10 + (m[3] / 10000) % 10 + (m[4] / 10000) % 10 != sum) return 0;
        // проверим диагонали
        if (m[0] % 10 + (m[1] / 10) % 10 + (m[2] / 100) % 10 + (m[3] / 1000) % 10 + (m[4] / 10000) % 10 != sum) return 0;
        if ((m[0] / 10000) % 10 + (m[1] / 1000) % 10 + (m[2] / 100) % 10 + (m[3] / 10) % 10 + m[4] % 10 != sum) return 0;
    }
    if (line == 4) {
        // проверим вертикали
        if (m[0] % 10 + m[1] % 10 + m[2] % 10 + m[3] % 10 + 1 <= sum > m[0] % 10 + m[1] % 10 + m[2] % 10 + m[3] % 10 + 9) return 0;
        if ((m[0] / 10) % 10 + (m[1] / 10) % 10 + (m[2] / 10) % 10 + (m[3] / 10) % 10 + 1 <= sum > (m[0] / 10) % 10 + (m[1] / 10) % 10 + (m[2] / 10) % 10 + (m[3] / 10) % 10 + 9) return 0;
        if ((m[0] / 100) % 10 + (m[1] / 100) % 10 + (m[2] / 100) % 10 + (m[3] / 100) % 10 + 1 <= sum > (m[0] / 100) % 10 + (m[1] / 100) % 10 + (m[2] / 100) % 10 + (m[3] / 100) % 10 + 9) return 0;
        if ((m[0] / 1000) % 10 + (m[1] / 1000) % 10 + (m[2] / 1000) % 10 + (m[3] / 1000) % 10 + 1 <= sum > (m[0] / 1000) % 10 + (m[1] / 1000) % 10 + (m[2] / 1000) % 10 + (m[3] / 1000) % 10 + 9) return 0;
        if ((m[0] / 10000) % 10 + (m[1] / 10000) % 10 + (m[2] / 10000) % 10 + (m[3] / 10000) % 10 + 1 <= sum > (m[0] / 10000) % 10 + (m[1] / 10000) % 10 + (m[2] / 10000) % 10 + (m[3] / 10000) % 10 + 9) return 0;
        // проверим диагонали
        if (m[0] % 10 + (m[1] / 10) % 10 + (m[2] / 100) % 10 + (m[3] / 1000) % 10 + 1 <= sum > m[0] % 10 + (m[1] / 10) % 10 + (m[2] / 100) % 10 + (m[3] / 1000) % 10 + 9) return 0;
        if ((m[0] / 10000) % 10 + (m[1] / 1000) % 10 + (m[2] / 100) % 10 + (m[3] / 10) % 10 + 1 <= sum > (m[0] / 10000) % 10 + (m[1] / 1000) % 10 + (m[2] / 100) % 10 + (m[3] / 10) % 10 + 9) return 0;
    }
    if (line == 3) {
        // проверим вертикали
        if (m[0] % 10 + m[1] % 10 + m[2] % 10 + 1 <= sum > m[0] % 10 + m[1] % 10 + m[2] % 10 + 9 * 2) return 0;
        if ((m[0] / 10) % 10 + (m[1] / 10) % 10 + (m[2] / 10) % 10 + 1 <= sum > (m[0] / 10) % 10 + (m[1] / 10) % 10 + (m[2] / 10) % 10+ 9 * 2) return 0;
        if ((m[0] / 100) % 10 + (m[1] / 100) % 10 + (m[2] / 100) % 10 + 1 <= sum > (m[0] / 100) % 10 + (m[1] / 100) % 10 + (m[2] / 100) % 10 + 9 * 2) return 0;
        if ((m[0] / 1000) % 10 + (m[1] / 1000) % 10 + (m[2] / 1000) % 10 + 1 <= sum > (m[0] / 1000) % 10 + (m[1] / 1000) % 10 + (m[2] / 1000) % 10 + 9 * 2) return 0;
        if ((m[0] / 10000) % 10 + (m[1] / 10000) % 10 + (m[2] / 10000) % 10 + 1 <= sum > (m[0] / 10000) % 10 + (m[1] / 10000) % 10 + (m[2] / 10000) % 10 + 9 * 2) return 0;
        // проверим диагонали
        if (m[0] % 10 + (m[1] / 10) % 10 + (m[2] / 100) % 10 + 1 <= sum > m[0] % 10 + (m[1] / 10) % 10 + (m[2] / 100) % 10 + 9 * 2) return 0;
        if ((m[0] / 10000) % 10 + (m[1] / 1000) % 10 + (m[2] / 100) % 10 + 1 <= sum > (m[0] / 10000) % 10 + (m[1] / 1000) % 10 + (m[2] / 100) % 10 + 9 * 2) return 0;
    }
    if (line == 2) {
        // проверим вертикали
        if (m[0] % 10 + m[1] % 10 + 1 <= sum > m[0] % 10 + m[1] % 10 + 9 * 3) return 0;
        if ((m[0] / 10) % 10 + (m[1] / 10) % 10 + 1 <= sum > (m[0] / 10) % 10 + (m[1] / 10) % 10 + 9 * 3) return 0;
        if ((m[0] / 100) % 10 + (m[1] / 100) % 10 + 1 <= sum > (m[0] / 100) % 10 + (m[1] / 100) % 10 + 9 * 3) return 0;
        if ((m[0] / 1000) % 10 + (m[1] / 1000) % 10 + 1 <= sum > (m[0] / 1000) % 10 + (m[1] / 1000) % 10 + 9 * 3) return 0;
        if ((m[0] / 10000) % 10 + (m[1] / 10000) % 10 + 1 <= sum > (m[0] / 10000) % 10 + (m[1] / 10000) % 10 + 9 * 3) return 0;
        // проверим диагонали
        if (m[0] % 10 + (m[1] / 10) % 10 + 1 <= sum > m[0] % 10 + (m[1] / 10) % 10 + 9 * 3) return 0;
        if ((m[0] / 10000) % 10 + (m[1] / 1000) % 10 + 1 <= sum > (m[0] / 10000) % 10 + (m[1] / 1000) % 10 + 9 * 3) return 0;
    }
    if (line == 1) {
        // проверим вертикали
        if (m[0] % 10 + 1 <= sum > m[0] % 10 + 9 * 4) return 0;
        if ((m[0] / 10) % 10 + 1 <= sum > (m[0] / 10) % 10 + 9 * 4) return 0;
        if ((m[0] / 100) % 10 + 1 <= sum > (m[0] / 100) % 10 + 9 * 4) return 0;
        if ((m[0] / 1000) % 10 + 1 <= sum > (m[0] / 1000) % 10 + 9 * 4) return 0;
        if ((m[0] / 10000) % 10 + 1 <= sum > (m[0] / 10000) % 10 + 9 * 4) return 0;
        // проверим диагонали
        if ((m[0] % 10 + 1 <= sum > m[0] % 10 + 9 * 4)) return 0;
        if (((m[0] / 10000) % 10 + 1 <= sum > (m[0] / 10000) % 10 + 9 * 4)) return 0;
    }
    return 1;
}

// проверяем являются ли вертикали и диагонали в матрице простыми числами, если матрица не достроена - есть ли нужные простые числа, начинающиеся с тех же цифр
int check_primes(int m[5], int line, int primes[10000]) {
    if (line == 0) return 1;
    if (line == 1) {
        if (!cache[m[0] % 10]) return 0;
        if (!cache[(m[0] / 10) % 10]) return 0;
        if (!cache[(m[0] / 100) % 10]) return 0;
        if (!cache[(m[0] / 1000) % 10]) return 0;
        if (!cache[(m[0] / 10000) % 10]) return 0;
        if (!cache[(m[0] / 10000) % 10]) return 0;
        if (!prime_ends[m[0] % 10]) return 0;
        return 1; }
    if (line == 2) {
        if (!cache[m[0] % 10 * 10 + m[1] % 10]) return 0;
        if (!cache[(m[0] / 10) % 10 * 10 + (m[1] / 10) % 10]) return 0;
        if (!cache[(m[0] / 100) % 10 * 10 + (m[1] / 100) % 10]) return 0;
        if (!cache[(m[0] / 1000) % 10 * 10 + (m[1] / 1000) % 10]) return 0;
        if (!cache[(m[0] / 10000) % 10 * 10 + (m[1] / 10000) % 10]) return 0;
        if (!prime_ends[m[0] % 10 + (m[1] / 10) % 10 * 10]) return 0;
        if (!cache[(m[0] / 10000) % 10 * 10 + (m[1] / 1000) % 10]) return 0;
        return 1;}
    if (line == 3) {
        if (!cache[m[0] % 10 * 100 + m[1] % 10 * 10 + m[2] % 10]) return 0;
        if (!cache[(m[0] / 10) % 10 * 100 + (m[1] / 10) % 10 * 10 + (m[2] / 10) % 10]) return 0;
        if (!cache[(m[0] / 100) % 10 * 100 + (m[1] / 100) % 10 * 10 + (m[2] / 100) % 10]) return 0;
        if (!cache[(m[0] / 1000) % 10 * 100 + (m[1] / 1000) % 10 * 10 + (m[2] / 1000) % 10]) return 0;
        if (!cache[(m[0] / 10000) % 10 * 100 + (m[1] / 10000) % 10 * 10 + (m[2] / 10000) % 10]) return 0;
        if (!prime_ends[m[0] % 10 + (m[1] / 10) % 10 * 10 + (m[2] / 100) % 10 * 100]) return 0;
        if (!cache[(m[0] / 10000) % 10 * 100 + (m[1] / 1000) % 10 * 10 + (m[2] / 100) % 10]) return 0;
        return 1; }
    if (line == 4) {
        if (!cache[m[0] % 10 * 1000 + m[1] % 10 * 100 + m[2] % 10 * 10 + m[3] % 10]) return 0;
        if (!cache[(m[0] / 10) % 10 * 1000 + (m[1] / 10) % 10 * 100 + (m[2] / 10) % 10 * 10 + (m[3] / 10) % 10]) return 0;
        if (!cache[(m[0] / 100) % 10 * 1000 + (m[1] / 100) % 10 * 100 + (m[2] / 100) % 10 * 10 + (m[3] / 100) % 10]) return 0;
        if (!cache[(m[0] / 1000) % 10 * 1000 + (m[1] / 1000) % 10 * 100 + (m[2] / 1000) % 10 * 10 + (m[3] / 1000) % 10]) return 0;
        if (!cache[(m[0] / 10000) % 10 * 1000 + (m[1] / 10000) % 10 * 100 + (m[2] / 10000) % 10 * 10 + (m[3] / 10000) % 10]) return 0;
        if (!prime_ends[m[0] % 10 + (m[1] / 10) % 10 * 10 + (m[2] / 100) % 10 * 100 + (m[3] / 1000) % 10 * 1000]) return 0;
        if (!cache[(m[0] / 10000) % 10 * 1000 + (m[1] / 1000) % 10 * 100 + (m[2] / 100) % 10 * 10 + (m[3] / 10) % 10]) return 0;
        return 1; }
    if (line == 5) {
        if (!cache[m[0] % 10 * 10000 + m[1] % 10 * 1000 + m[2] % 10 * 100 + m[3] % 10 * 10 + m[4] % 10]) return 0;
        if (!cache[(m[0] / 10) % 10 * 10000 + (m[1] / 10) % 10 * 1000 + (m[2] / 10) % 10 * 100 + (m[3] / 10) % 10 * 10 + (m[4] / 10) % 10]) return 0;
        if (!cache[(m[0] / 100) % 10 * 10000 + (m[1] / 100) % 10 * 1000 + (m[2] / 100) % 10 * 100 + (m[3] / 100) % 10 * 10 + (m[4] / 100) % 10]) return 0;
        if (!cache[(m[0] / 1000) % 10 * 10000 + (m[1] / 1000) % 10 * 1000 + (m[2] / 1000) % 10 * 100 + (m[3] / 1000) % 10 * 10 + (m[4] / 1000) % 10]) return 0;
        if (!cache[(m[0] / 10000) % 10 * 10000 + (m[1] / 10000) % 10 * 1000 + (m[2] / 10000) % 10 * 100 + (m[3] / 10000) % 10 * 10 + (m[4] / 10000) % 10]) return 0;
        if (!cache[m[0] % 10 + (m[1] / 10) % 10 * 10 + (m[2] / 100) % 10 * 100 + (m[3] / 1000) % 10 * 1000 + m[4] / 10000 * 10000]) return 0;
        if (!cache[(m[0] / 10000) % 10 * 10000 + (m[1] / 1000) % 10 * 1000 + (m[2] / 100) % 10 * 100 + (m[3] / 10) % 10 * 10 + m[4] % 10]) return 0;
        return 1;
    }
}

// функция перебора
void backtracking(int m[5], int line, int sum, int first, int primes[10000], FILE *fout){
    if (check_primes(m, line, primes) && check_sums(m, sum, line)) { // проверяем матрицу на соответствие условиям задачи
        if (line == 5) { // если матрица заполнена - выводим
            matrix_output(m, fout);
        }
        else // иначе перебираем возможные значения незаполненных строк
            for (int i = 0; primes[i]; i++) {
                if (line == 0 && primes[i] / 10000 < first) continue; // если первая цифра меньше нужной пропускаем шаг
                if (line == 0 && primes[i] / 10000 > first) return; // если первая цифра больше => прекращаем поиск
                if (line == 4 && if_even_in_prime(primes[i])) continue; // если есть четные цифры в числе для последней строки
                m[line] = primes[i]; // подставляем возможное значение
                backtracking(m, line + 1, sum, first, primes, fout); // запускаем дальшейший перебор с этим значением
            }
    }
    else return;
}



int main() {
    FILE *fin, *fout;
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w+");
    int sum, first;
    fscanf(fin, "%d\n%d", &sum, &first);
    int matrix[5] = { 0 };
    int primes[10000] = { 0 };
    find_suitable_primes(primes, sum);
    double time_spent = 0.0;
    clock_t begin = clock();

    backtracking(matrix, 0, sum, first, primes, fout);

    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Время работы функции перебора %f секунд\n", time_spent);
    printf("Количество ответов: %d ", matrix_count);
    return 0;
}
