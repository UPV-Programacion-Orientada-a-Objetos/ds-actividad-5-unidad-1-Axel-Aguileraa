#include <iostream>
#include <iomanip> 


template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:
    MatrizBase(int f, int c) : _filas(f), _columnas(c) {}

    virtual ~MatrizBase() {}

    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;

    virtual T getValor(int f, int c) const = 0;
    virtual void setValor(int f, int c, T valor) = 0;

    int getFilas() const { return _filas; }
    int getColumnas() const { return _columnas; }
};


template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T **_datos;

    void asignarMemoria() {
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; ++i) {
            _datos[i] = new T[this->_columnas];
        }
    }

    void liberarMemoria() {
        if (_datos != nullptr) {
            for (int i = 0; i < this->_filas; ++i) {
                delete[] _datos[i];
            }
            delete[] _datos;
            _datos = nullptr;
        }
    }

public:
    MatrizDinamica(int f, int c) : MatrizBase<T>(f, c), _datos(nullptr) {
        asignarMemoria();
    }

    virtual ~MatrizDinamica() {
        std::cout << "Liberando Matriz Dinámica (" << this->_filas << "x" << this->_columnas << ")...\n";
        liberarMemoria();
    }

    MatrizDinamica(const MatrizDinamica<T>& other)
        : MatrizBase<T>(other._filas, other._columnas) {
        asignarMemoria();
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                _datos[i][j] = other._datos[i][j];
            }
        }
    }

    MatrizDinamica<T>& operator=(const MatrizDinamica<T>& other) {
        if (this == &other) return *this; 

        liberarMemoria();
        this->_filas = other._filas;
        this->_columnas = other._columnas;
        asignarMemoria();

        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                _datos[i][j] = other._datos[i][j];
            }
        }
        return *this;
    }

    MatrizDinamica(MatrizDinamica<T>&& other) noexcept
        : MatrizBase<T>(other._filas, other._columnas), _datos(other._datos) {
        other._datos = nullptr;
        other._filas = 0;
        other._columnas = 0;
    }

    MatrizDinamica<T>& operator=(MatrizDinamica<T>&& other) noexcept {
        if (this == &other) return *this;
        liberarMemoria(); 
        this->_filas = other._filas;
        this->_columnas = other._columnas;
        _datos = other._datos;
        other._datos = nullptr;
        other._filas = 0;
        other._columnas = 0;
        return *this;
    }


    virtual void cargarValores() override {
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                _datos[i][j] = T(); 
            }
        }
    }

    virtual void imprimir() const override {
        for (int i = 0; i < this->_filas; ++i) {
            std::cout << "|";
            for (int j = 0; j < this->_columnas; ++j) {
                std::cout << std::setw(6) << _datos[i][j] << " |";
            }
            std::cout << "\n";
        }
    }

    virtual T getValor(int f, int c) const override { return _datos[f][c]; }
    virtual void setValor(int f, int c, T valor) override { _datos[f][c] = valor; }

    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            return nullptr; 
        }

        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(this->_filas, this->_columnas);
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                T suma = this->getValor(i, j) + otra.getValor(i, j);
                resultado->setValor(i, j, suma);
            }
        }
        return resultado;
    }
};

template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
private:
    T _datos[M][N]; 

public:
    MatrizEstatica() : MatrizBase<T>(M, N) {}

    virtual ~MatrizEstatica() {
        std::cout << "Liberando Matriz Estática (" << M << "x" << N << ")...\n";
    }


    virtual void cargarValores() override {
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                _datos[i][j] = T(); 
            }
        }
    }

    virtual void imprimir() const override {
        for (int i = 0; i < M; ++i) {
            std::cout << "|";
            for (int j = 0; j < N; ++j) {
                std::cout << std::setw(6) << _datos[i][j] << " |";
            }
            std::cout << "\n";
        }
    }

    virtual T getValor(int f, int c) const override { return _datos[f][c]; }
    virtual void setValor(int f, int c, T valor) override { _datos[f][c] = valor; }

    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            return nullptr;
        }

        MatrizEstatica<T, M, N>* resultado = new MatrizEstatica<T, M, N>();
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                T suma = this->getValor(i, j) + otra.getValor(i, j);
                resultado->setValor(i, j, suma);
            }
        }
        return resultado;
    }
};


template <typename T>
MatrizBase<T>* operator+(const MatrizBase<T>& a, const MatrizBase<T>& b) {
    return a.sumar(b);
}



int main() {
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "--- Sistema gérico de Álgebra Lineal --->\n\n";

 
    
    std::cout << ">> Demostración de Genericidad (Tipo FLOAT) <<\n\n";

    std::cout << "// 1. Creación de Matriz Dinámica A (3x2)...\n";
    MatrizBase<float>* A = new MatrizDinamica<float>(3, 2);
    A->setValor(0, 0, 1.5f); A->setValor(0, 1, 2.0f);
    A->setValor(1, 0, 0.0f); A->setValor(1, 1, 1.0f);
    A->setValor(2, 0, 4.5f); A->setValor(2, 1, 3.0f);
    std::cout << "A =\n";
    A->imprimir();
    std::cout << "\n";


    std::cout << "// 2. Creación de Matriz Estática B (3x2)...\n";
    MatrizBase<float>* B = new MatrizEstatica<float, 3, 2>();
    B->setValor(0, 0, 0.5f); B->setValor(0, 1, 1.0f);
    B->setValor(1, 0, 2.0f); B->setValor(1, 1, 3.0f);
    B->setValor(2, 0, 1.0f); B->setValor(2, 1, 1.0f);
    std::cout << "B =\n";
    B->imprimir();
    std::cout << "\n";

    std::cout << "// 3. Operación Polimórfica (Suma)\n";
    std::cout << "SUMANDO: Matriz C = A + B ...\n";
    std::cout << "(Suma manejada por el método virtual de MatrizDinamica)\n\n";

    MatrizBase<float>* C = *A + *B; 

    if (C) {
        std::cout << "Matriz Resultado C (3x2, Tipo FLOAT):\n";
        C->imprimir();
    }

    std::cout << "\n\n>> Demostración de Genericidad (Tipo INT) <<\n\n";
    
    MatrizBase<int>* D = new MatrizEstatica<int, 2, 2>();
    D->setValor(0, 0, 10); D->setValor(0, 1, 20);
    D->setValor(1, 0, 30); D->setValor(1, 1, 40);
    std::cout << "D (Estatica) =\n";
    D->imprimir();
    std::cout << "\n";

    MatrizBase<int>* E = new MatrizDinamica<int>(2, 2);
    E->setValor(0, 0, 5); E->setValor(0, 1, 4);
    E->setValor(1, 0, 3); E->setValor(1, 1, 2);
    std::cout << "E (Dinamica) =\n";
    E->imprimir();
    std::cout << "\n";

    std::cout << "SUMANDO: Matriz F = D + E ...\n";
    std::cout << "(Suma manejada por el método virtual de MatrizEstatica)\n\n";
    MatrizBase<int>* F = *D + *E; 
    if(F){
        std::cout << "Matriz Resultado F (2x2, Tipo INT):\n";
        F->imprimir();
    }

    // 

    std::cout << "\n\n>> Limpieza de Memoria <<\n";
    
    std::cout << "Llamando al destructor de C (Dinamica)...\n";
    delete C; 
    std::cout << "Llamando al destructor de A (Dinamica)...\n";
    delete A; 
    std::cout << "Llamando al destructor de B (Estatica)...\n";
    delete B; 

    std::cout << "\nLlamando al destructor de F (Estatica)...\n";
    delete F;
    std::cout << "Llamando al destructor de D (Estatica)...\n";
    delete D;
    std::cout << "Llamando al destructor de E (Dinamica)...\n";
    delete E;
    
    std::cout << "\nSistema cerrado.\n";

    return 0;
}