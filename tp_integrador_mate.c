#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h> // Para funciones como isdigit()
#include <string.h>

// Definición de la estructura de la pila
typedef struct {
    char *items;
    int top;
    int capacidad;
} Stack;

// Función para inicializar la pila
Stack *crear_pila(int capacidad) {
    Stack *pila = (Stack *)malloc(sizeof(Stack));
    pila->capacidad = capacidad;
    pila->top = -1;
    pila->items = (char *)malloc(capacidad * sizeof(char));
    return pila;
}

// Función para verificar si la pila está vacía
bool pila_vacia(Stack *pila) {
    return pila->top == -1;
}

// Función para verificar si la pila está llena
bool pila_llena(Stack *pila) {
    return pila->top == pila->capacidad - 1;
}

// Función para agregar un elemento a la pila
void push(Stack *pila, char item) {
    if (!pila_llena(pila)) {
        pila->items[++pila->top] = item;
    }
}

// Función para eliminar un elemento de la pila y devolverlo
char pop(Stack *pila) {
    if (!pila_vacia(pila)) {
        return pila->items[pila->top--];
    }
    return '\0'; // En caso de pila vacía
}

// Función para obtener el elemento superior de la pila sin eliminarlo
char peek(Stack *pila) {
    if (!pila_vacia(pila)) {
        return pila->items[pila->top];
    }
    return '\0'; // En caso de pila vacía
}

// Función para determinar la precedencia de los operadores
int precedencia(char operador) {
    switch (operador) {
        case '|':
            return 1;
        case '&':
            return 2;
        case '!':
            return 3;
        default:
            return 0;
    }
}

// Función para convertir la expresión de infix a postfix (notación postfija)
void infix_a_postfix(const char *expresion, char *expresion_postfix) {
    Stack *pila = crear_pila(strlen(expresion));
    int j = 0; // Índice para la expresión_postfix

    for (int i = 0; expresion[i]; i++) {
        if (isdigit(expresion[i])) {
            expresion_postfix[j++] = expresion[i];
        } else if (expresion[i] == '&' || expresion[i] == '|' || expresion[i] == '!') {
            while (!pila_vacia(pila) && precedencia(peek(pila)) >= precedencia(expresion[i])) {
                expresion_postfix[j++] = pop(pila);
            }
            push(pila, expresion[i]);
        }
    }

    // Vaciar la pila
    while (!pila_vacia(pila)) {
        expresion_postfix[j++] = pop(pila);
    }
    expresion_postfix[j] = '\0';

    free(pila->items);
    free(pila);
}

// Función para evaluar la expresión postfija
bool evaluar_expresion_postfix(const char *expresion_postfix) {
    Stack *pila = crear_pila(strlen(expresion_postfix));

    for (int i = 0; expresion_postfix[i]; i++) {
        if (isdigit(expresion_postfix[i])) {
            push(pila, expresion_postfix[i] - '0'); // Convertir char a int
        } else if (expresion_postfix[i] == '&' || expresion_postfix[i] == '|') {
            bool op2 = pop(pila);
            bool op1 = pop(pila);

            switch (expresion_postfix[i]) {
                case '&':
                    push(pila, op1 && op2);
                    break;
                case '|':
                    push(pila, op1 || op2);
                    break;
                default:
                    break;
            }
        } else if (expresion_postfix[i] == '!') {
            bool op = pop(pila);
            push(pila, !op);
        }
    }

    bool resultado = pop(pila);
    free(pila->items);
    free(pila);

    return resultado;
}

int main() {
    char expresion[50]; // Array para almacenar la expresión ingresada
    int continuar;

    do {
        printf("\n\n\nIngresa la expresion booleana en formato 1 (verdadero) o 0 (falso) con las expresiones &; |; !:\n");
        fgets(expresion, sizeof(expresion), stdin);
        expresion[strcspn(expresion, "\n")] = '\0';
        // Eliminar el salto de línea del final de la entrada

        // Convertir la expresión infix a postfix
        char expresion_postfix[50];
        infix_a_postfix(expresion, expresion_postfix);

        // Evaluar la expresión postfija
        bool resultado = evaluar_expresion_postfix(expresion_postfix);

        // Mostrar resultado
        if (resultado) {
            printf("\nLa expresion es VERDADERA.\n");
        } else {
            printf("\nLa expresion es FALSA.\n");
        }

        // Pedir al usuario si desea ingresar otra expresión
        printf("\nDeseas ingresar otra expresion? (1=Si/2=No):\n");
        if  (scanf("%d", &continuar) != 1) {
            printf("\nError. Introduce un valor numerico\n");
            exit(1);
        }

        while(getchar() != '\n');

    } while (continuar == 1);

    printf("\nFinalizo el programa.\n\n");

    return 0;
}
