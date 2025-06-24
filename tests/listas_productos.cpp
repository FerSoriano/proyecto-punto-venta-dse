#include <iostream>
#include <string>

using namespace std;

// strucs
struct Usuario{
    string usuario;
    string pass;
    int tipo;
    int status;
};

struct Nodo{
    Usuario usuario;
    Nodo* next;
};

// prototipos
Nodo* inicializarListaUsuarios();
void mostrarUsuarios(Nodo*& lista);
bool agregarUsuario(Nodo*& lista, const Usuario& usuario);
Usuario crearNuevoUsuario(const string&);

// variables globales
int totalUsuarios = 3;

int main(){
    Nodo* listaUsuarios = inicializarListaUsuarios();
    mostrarUsuarios(listaUsuarios);

    string nombreUsuario;
    while(true){
        cout << "\n\n--- ALTA USUARIOS ---\n\n";
        cout << "Nombre: "; cin >> nombreUsuario;
        if(nombreUsuario == "*") break;
        Usuario nuevoUsuario = crearNuevoUsuario(nombreUsuario);
        agregarUsuario(listaUsuarios, nuevoUsuario);
    }
    cout << "\n";

    mostrarUsuarios(listaUsuarios);

    return 0;
}

// -------- FUNCIONES

Nodo* inicializarListaUsuarios(){
    Usuario usuarios[3] = {
        {"admin", "123", 1, 1}, 
        {"vend1", "123", 2, 1}, 
        {"vend2", "123", 2, 1}
    };

    Nodo* listaUsuarios = NULL;
    Nodo* ptr = listaUsuarios;

    for(int i=0; i < totalUsuarios; i++){
        agregarUsuario(listaUsuarios,usuarios[i]);
    }
    return listaUsuarios;
}


bool agregarUsuario(Nodo*& lista,const Usuario& usuario){
    Nodo* nuevoUsuario = new Nodo();
    nuevoUsuario->usuario = usuario;

    Nodo* aux1 = lista;
    Nodo* aux2;

    while(aux1 != NULL){
        aux2 = aux1;
        aux1 = aux1->next;
    }

    if(aux1 == lista){ // aux siempre sera null, si son iguales significa que la lista esta vacia y se agrega el primer nodo
        lista = nuevoUsuario;
    }else{
        aux2->next = nuevoUsuario;
    }

    nuevoUsuario->next = aux1;

    return true;
}

void mostrarUsuarios(Nodo*& lista){
    Nodo* ptr = lista;

    cout << "\nLista de Usuarios:" << endl;
    while(ptr != NULL){
        cout << ptr->usuario.usuario << ", " << ptr->usuario.pass << ", " << ptr->usuario.tipo << ", " << ptr->usuario.status << endl;
        ptr = ptr->next;
    }
    cout << "\n";
}

Usuario crearNuevoUsuario(const string& nombreUsuario){
    Usuario nuevoUsuario;
    nuevoUsuario.usuario = nombreUsuario;
    cout << "Contra: "; cin >> nuevoUsuario.pass;
    cout << "Tipo (1 admin / 2 vend): "; cin >> nuevoUsuario.tipo;
    nuevoUsuario.status = 1;

    return nuevoUsuario;
}