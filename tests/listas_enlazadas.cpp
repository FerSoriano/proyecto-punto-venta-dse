#include <iostream>
#include <string>
#include <list>

using namespace std;

struct Nodo{
    int valor;
    Nodo* siguiente;
};

void insertarNumeroEnLista(Nodo*& lista, int valor);
void mostrarElementosLista(Nodo*& lista);
bool eliminarElementoLista(Nodo*& lista, int num);
bool editarElementoLista(Nodo*& lista, int num, int nuevoNumero);
void ordenarLista(Nodo*& lista);

int main(){
    Nodo* lista = NULL; // inicializamos lista

    int num;
    for(int i = 0; i < 5; i++){ // agregamos valores
        cout << "Numero: ";
        cin >> num;
        insertarNumeroEnLista(lista, num);
    }

    mostrarElementosLista(lista);

    cout << "\nInserta un numero mas: "; cin >> num;
    insertarNumeroEnLista(lista, num);
    mostrarElementosLista(lista);

    // eliminar un numero de la lista
    cout << "\nElimina un numero de la lista: "; cin >> num;
    if(eliminarElementoLista(lista, num)){
        cout << "\nEl numero " << num << " fue eliminado!\n";
        mostrarElementosLista(lista);
    } else {
        cout << "\nNo se encontro el numero " << num << endl;
    }

    // editar un elemento
    int nuevoNum;
    cout << "\nEscribe el numero que quieres editar: "; cin >> num;
    cout << "Escribe el nuevo numero: "; cin >> nuevoNum;
    if(editarElementoLista(lista, num, nuevoNum)){
        cout << "\nSe modifico el " << num << " por el " << nuevoNum << endl;
        mostrarElementosLista(lista);
        ordenarLista(lista);
        mostrarElementosLista(lista);
    } else {
        cout << "\nNo se encontro el numero " << num << endl;
    }


    return 0;
}

void insertarNumeroEnLista(Nodo*& lista, int num){
    Nodo* nuevo_nodo = new Nodo();
    nuevo_nodo->valor = num;

    Nodo* aux1 = lista;
    Nodo* aux2;

    while(aux1 != NULL && aux1->valor < num){
        aux2 = aux1;
        aux1 = aux1->siguiente;
    }
    
    if(lista == aux1){ // si se cumple, se inserta al principio
        lista = nuevo_nodo;
    } else {
        aux2->siguiente = nuevo_nodo;
    }

    nuevo_nodo->siguiente = aux1; // en este punto, aux1 sera el resto de la lista o NULL.
}

void ordenarLista(Nodo*& head){
    Nodo* aux1 = head; 
    Nodo* aux2;

    while(aux1 != NULL){
        aux2 = aux1->siguiente;
        while(aux2 != NULL){
            if(aux1->valor > aux2->valor){
                int tmp = aux1->valor;
                aux1->valor = aux2->valor;
                aux2->valor = tmp;
            }
            aux2 = aux2->siguiente;
        }
        aux1 = aux1->siguiente;
    } 
}

void mostrarElementosLista(Nodo*& lista){
    Nodo* aux1 = lista;

    cout << "\nLista de numeros:" << endl;
    while(aux1 != NULL){
        cout << aux1->valor << " ";
        aux1 = aux1->siguiente;
    }
    cout << "\n";
}

bool eliminarElementoLista(Nodo*& lista, int num){
    Nodo* aux = lista;
    Nodo* previo;
    
    while(aux != NULL && aux->valor != num){
        previo = aux; // se almacena el nodo previo antes de encontrar el valor
        aux = aux->siguiente;
    }
    
    if(aux == NULL){
        return false;  // no se encontro el numero
    }

    if(aux == lista){ // estamos en head
        lista = aux->siguiente; // movemos el head al siguiente nodo.
    }else{
        previo->siguiente = aux->siguiente; // cambiamos el puntero al siguiente nodo
    }

    delete aux; // eliminamos el nodo

    return true;
}

bool editarElementoLista(Nodo*& lista, int num, int nuevoNumero){
    Nodo* aux = lista;

    while(aux != NULL && aux->valor != num){
        aux = aux->siguiente;
    }

    if(aux == NULL){
        return false; // no se encontro el numero
    }

    aux->valor = nuevoNumero;
    return true;
}

