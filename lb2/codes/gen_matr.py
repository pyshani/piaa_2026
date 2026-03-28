import random
import os

def main():
    n = int(input("Введите размер квадратной матрицы: "))

    matrix = [[random.randint(0, 100) for _ in range(n)] for _ in range(n)]

    os.makedirs("tests", exist_ok=True)

    with open("tests/testr", "w", encoding="utf-8") as f:
        f.write(f"{n}\n")
        for row in matrix:
            f.write(" ".join(map(str, row)) + "\n")

if __name__ == "__main__":
    main()