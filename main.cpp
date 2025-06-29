#include <iostream>
#include <string>
#include <iomanip> // uso de setw()
#include <sstream>

using namespace std;

// definimos el Struct para almacenar los usuarios, productos y ventas

struct Usuario{
    string usuario;
    string pass;
    int tipo;
    int status;
};

struct Producto{
    int id;
    string producto;
    float pc;
    float pv;
    int existencias;
    int nivelReorden;
    int status;
};

struct Venta{
    string vendedor;
    string productos[100];
    int cantidad[100];
    float pc[100];
    float pv[100];
    int totalProductosVentas = 0;
};

// Nueva estructura para manejar Usuarios con Nodos
struct NodoUsuario{
    Usuario usuario;
    NodoUsuario* next;
};


// Prototipos funciones

int menuPrincipal();
void menuAdmin();
bool validarLogin(int tipoUsuario);
void validarInput();
void mostrarInventario();
void mostrarProductos(int tipoOrden);
bool altaProducto(string& nombreProducto, float& pc, float& pv, int& existencia, int& nivelReorden);
Producto* buscarProducto(const string& nombreProducto);
NodoUsuario* buscarUsuario(NodoUsuario*& lista, string nombreUsuario);
string convertirMinus(string str);
bool consultarProducto(Producto* producto);
bool modificarProducto(Producto* producto, const string& campoModificar, float valor);
bool bajaProducto(Producto* producto);
void mostrarMenuAdminCuentasUsuario();
void altaUsuario(NodoUsuario* nodo, string nombreUsuario);
void bajaUsuario(NodoUsuario* nodo, string nombreUsuario);
void modificarUsuario(NodoUsuario* nodo, string nombreUsuario);
void consultarUsuario(NodoUsuario* nodo, string nombreUsuario);
void mostrarCuentasUsuarios();
void hacerVenta();
void imprimirTicket(const Venta& venta);
void restarInventario(const int& id, const int& cantidad);
void reiniciarVenta(Venta& venta);
void corteCajaVendedor(const string& vendedor);
void corteCajaGeneral();
void limpiarConsola();
void ordernarNombre();
void ordernarId();
void mostrarEncabezadosProducto();
void mostrarInfoProducto(const Producto& producto);
void mostrarEncabezadosUsuario();
void mostrarInfoUsuario(const Usuario& usuario);
int contarUsuariosAdmin();
string formatoMoneda(float monto);
bool reactivarProducto(Producto* producto);
float solicitarNumeroAlUsuario(const string& mensajeEntrada, const float& valorMin,const float& valorMax, const string& mensajeError, bool cambiarSigno);
string solicitarProductoAlUsuario(const string& mensajeEntrada);
void solicitarYActualizarCampoProducto(Producto* producto, const string&textoActual, float valorActual, const string&textoNuevo, const string& campoModificar, const string& textoExito);
void menuModificaciones(Producto* producto);

// NODOS
NodoUsuario* inicializarListaUsuarios();
bool agregarUsuarioLista(NodoUsuario*& head, const Usuario& usuario);

// variables globales

int totalUsuarios = 3;
int totalProductos = 5;
Producto productos[100] = {
    {1,"Agua",13.39,18.55,12,4,1},
    {2,"Leche",12.35,15.5,16,5,1},
    {3,"Huevos",22.4,30.39,20,7,1},
    {4,"Pan",5.5,7.95,18,6,1},
    {5,"Refresco",10.99,14.75,30,8,1}
};

int totalVentas = 0;
Venta ventas[100];

string currentUser; // manejar al usuario que esta dentro del sistema

NodoUsuario* listaUsuarios = inicializarListaUsuarios();


int main(){
    bool isExcecute = true;
    int optionMenu;
    while (isExcecute){
        optionMenu = menuPrincipal();
        switch (optionMenu){
            case 1:
                menuAdmin();
                break;
            case 2:
                hacerVenta();
                break;
            case 3:
                cout << "\n-------------------------------------------------\n     SALIENDO DEL SISTEMA. FIN DEL PROGRAMA.\n-------------------------------------------------\n\n";
                isExcecute = false;
                break;
            default:
                limpiarConsola();
                cout << "\n*** Opcion invalida. Intenta de nuevo. ***";
                break;
        }
    }
    return 0;
}

NodoUsuario* inicializarListaUsuarios(){
    Usuario usuarios[3] = {
        {"admin", "123", 1, 1}, 
        {"vend1", "123", 2, 1}, 
        {"vend2", "123", 2, 1}
    };
    NodoUsuario* head = NULL;
    for(int i=0; i < totalUsuarios; i++){
        agregarUsuarioLista(head, usuarios[i]);
    } 
    return head;
}

bool agregarUsuarioLista(NodoUsuario*& head, const Usuario& usuario){
    NodoUsuario* nuevoUsuario = new NodoUsuario();
    nuevoUsuario->usuario = usuario;

    NodoUsuario* ptr = head;
    NodoUsuario* end;

    while(ptr != NULL){
        end = ptr;
        ptr = ptr->next;
    }

    if(ptr == head){ // aux siempre sera null, si son iguales significa que la lista esta vacia y se agrega el primer nodo
        head = nuevoUsuario;
    }else{
        end->next = nuevoUsuario;
    }

    nuevoUsuario->next = NULL;

    return true;
}

void limpiarConsola(){
    #ifdef _WIN32
        system("cls"); // Windows
    #else
        system("clear"); // macOS/Linux
    #endif
};

int menuPrincipal(){
    int option;
    cout << "\n\n\tMENU PRINCIPAL\n\n";
    cout << "1. Administrador\n2. Ventas\n3. Salir\n\n";
    cout << "\tOpcion: "; cin >> option;
    validarInput();
    return option;
}

void menuAdmin(){
    if (validarLogin(1)){
        limpiarConsola();
        int option;
        bool ejecutarMenu = true;
        string nombreProducto;
        float pc, pv;
        int existencia, nivelReorden;
        
        while(ejecutarMenu){
            cout << "\n\t\t\t\t\tUsuario: " << currentUser; //TODO: implementar en todos los menus?
            cout << "\n\n\tMENU ADMINISTRADOR\n\n";
            cout << "1. Altas\n2. Bajas\n3. Consultas\n4. Modificaciones\n5. Mostrar Inventario\n6. Administracion de Cuentas de Usuario\n7. Corte de caja general\n8. Regresar al menu anterior.\n\n";
            cout << "\tOpcion: ";
            cin >> option;
            validarInput();
            limpiarConsola();
            switch (option){
                case 1: // alta
                    while(true){
                        nombreProducto = solicitarProductoAlUsuario("ALTA DE PRODUCTO");
                        if (nombreProducto == "*"){limpiarConsola(); break;}

                        Producto* producto = buscarProducto(nombreProducto);
                        if(producto != nullptr){
                            if(producto->status == 0){
                                if(reactivarProducto(producto)){
                                    mostrarEncabezadosProducto();
                                    mostrarInfoProducto(*producto);
                                    cout << "\n\nEl producto " << producto->producto << " se dio de alta nuevamente.\n\n";
                                }
                            }else{
                                cout << "\n\n*** El producto \"" << nombreProducto << "\"  ya existe. Intenta de nuevo. ***\n\n\n";
                            }
                            continue;
                        }

                        pc = solicitarNumeroAlUsuario("Precio compra: ", 00.00, 99999.99, "\n\n*** Error en el precio. ***\n\n\n", false);
                        pv = solicitarNumeroAlUsuario("Precio venta: ", pc, 99999.99, "\n\n*** El Precio de Venta no puede ser menor al Precio de Compra. Intenta de nuevo. ***\n\n\n", true);
                        existencia = solicitarNumeroAlUsuario("Existencia: ", 00.00, 99999.99, "\n\n*** Error en la cantidad. ***\n\n\n", false);
                        nivelReorden = solicitarNumeroAlUsuario("Nivel de Reorden: ", 00.00, existencia,"\n\n*** El Nivel de Reorden no puede ser mayor que la Existencia. Intenta de nuevo. ***\n\n\n", false);

                        if(altaProducto(nombreProducto,pc,pv,existencia,nivelReorden)){
                            cout << "\n\nEl producto \"" << nombreProducto << "\" se agrego correctamente.\n\n";
                        }
                    }
                    break;
                case 2: // baja           
                    while(true){
                        nombreProducto = solicitarProductoAlUsuario("BAJA DE PRODUCTO");
                        if (nombreProducto == "*"){limpiarConsola(); break;}

                        Producto* producto = buscarProducto(nombreProducto);
                        if(bajaProducto(producto)){
                            cout << "\nEl producto \"" << producto->producto << "\" se dio de baja\n";
                        }else {
                            cout << "\n\n*** Producto \"" << nombreProducto << "\" no encontrado. Intenta de nuevo. ***\n\n"; 
                        }
                    }  
                    break;
                case 3: // consulta
                    while (true){
                        nombreProducto = solicitarProductoAlUsuario("CONSULTA DE PRODUCTO");
                        if (nombreProducto == "*"){limpiarConsola(); break;}
                        Producto* producto = buscarProducto(nombreProducto);
                        if(!consultarProducto(producto)){
                            cout << "\n\n*** No se encontro el producto \"" << nombreProducto << "\" ***\n\n";
                        }
                    }
                    break;
                case 4: // modificar
                    while (true){
                        nombreProducto = solicitarProductoAlUsuario("MODIFICACIONES");
                        if (nombreProducto == "*"){limpiarConsola(); break;}
                        
                        Producto* producto = buscarProducto(nombreProducto);
                        if(producto == nullptr || producto->status == 0){
                            cout << "\n\n*** No se encontro el producto \"" << nombreProducto << "\" ***\n\n";
                            continue;
                        }
                        menuModificaciones(producto);        
                    }
                    break;
                case 5:
                    mostrarInventario();
                    break;
                case 6:
                    mostrarMenuAdminCuentasUsuario();
                    break;
                case 7:
                    corteCajaGeneral();
                    break;
                case 8:
                    ejecutarMenu = false;
                    break;
                default:
                    cout << "\n*** Opcion invalida. Intenta de nuevo. ***";
                    break;
            }
        }
    }
}

string solicitarProductoAlUsuario(const string& mensajeEntrada){
    string nombreProducto;
    cout << "\n\n\t" << mensajeEntrada << "\n\n";
    cout << "Producto: "; cin >> nombreProducto;
    return nombreProducto;
}

bool reactivarProducto(Producto* producto){
    char res;
    cout << "\nEste producto estaba dado de baja, ¿Quieres darlo de alta nuevamente? (y/n): "; cin >> res; cout << '\n';
    if(tolower(res) == 'y'){
        producto->status = 1;
        return true;
    }
    return false;
}

float solicitarNumeroAlUsuario(const string& mensajeEntrada, const float& valorMin,const float& valorMax, const string& mensajeError, bool cambiarSigno){
    float valor;
    if(!cambiarSigno)
        do{
            cout << mensajeEntrada; cin >> valor; validarInput();
            if(valor <= valorMin){ cout << "\n\n*** Datos invalidos. Intenta de nuevo. ***\n\n\n"; continue; }
            if(valor > valorMax){ cout << mensajeError; }
        }while(valor <= valorMin || valor > valorMax);
    else{
        do{
            cout << mensajeEntrada; cin >> valor; validarInput();
            if(valor <= 0){ cout << "\n\n*** Datos invalidos. Intenta de nuevo. ***\n\n\n"; continue; }
            if(valor < valorMin){ cout << mensajeError; }
        }while(valor <= 0 || valor < valorMin);
    }
    return valor;
}

bool validarLogin(int tipoUsuario){
    bool isValid = false;
    string nombreUsuario, pass;
    do{
        cout << "\nUsuario: ";
        cin >> nombreUsuario;
        if(nombreUsuario == "*"){limpiarConsola(); break;}
        cout << "\nPassword: ";
        cin >> pass;

        NodoUsuario* nodo = buscarUsuario(listaUsuarios, nombreUsuario);

        if(nodo == nullptr){ cout << "\n\n*** Usuario o contraseña incorrectos. Intenta de nuevo. ***\n"; continue; }

        if (nodo->usuario.usuario == nombreUsuario && nodo->usuario.pass == pass && nodo->usuario.tipo == tipoUsuario && nodo->usuario.status == 1){
            currentUser = nodo->usuario.usuario;
            isValid = true;
            break;
        }

        if (isValid == false){ cout << "\n\n*** Usuario o contraseña incorrectos. Intenta de nuevo. ***\n"; }

    } while (!isValid);

    return isValid;
}

void validarInput(){
    // si el usuario ingresa un dato no numerico, se limpia el cache de cin.
    if (!cin.good()){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void mostrarInventario(){
    int option;
    
    while(true){
        cout << "\n\n\tMOSTRAR INVENTARIO\n\nOrdenado por:\n";
        cout << "1. Por Id\n2. Por producto\n3. Regresar al menú anterior.\n\n";
        cout << "\tOpcion: "; cin >> option;
        validarInput();
        limpiarConsola();
        if(option == 1 || option == 2) {
            mostrarProductos(option);
            continue;
        }

        if(option == 3){ break; }

        cout << "\n*** Opcion invalida. Intenta de nuevo. ***";
    }
}

// Bubble Sort
void ordernarNombre(){
    Producto tmp;
    for(int i=0; i < totalProductos - 1; i++){
        for(int j=0; j < totalProductos - i - 1; j++){
            if(convertirMinus(productos[j].producto) > convertirMinus(productos[j + 1].producto)){
                tmp = productos[j];
                productos[j] = productos[j + 1];
                productos[j + 1] = tmp;
            }
        }
    }
}

void ordernarId(){
    Producto tmp;
    for(int i=0; i < totalProductos - 1; i++){
        for(int j=0; j < totalProductos - i - 1; j++){
            if(productos[j].id > productos[j + 1].id){
                tmp = productos[j];
                productos[j] = productos[j + 1];
                productos[j + 1] = tmp;
            }
        }
    }
}

void mostrarProductos(int tipoOrden){
    if(tipoOrden == 1)
        ordernarId();
    else
        ordernarNombre();
    
    cout << "---------------------------------------------------------------------------------------\n\t\t\t\t\tINVENTARIO\n---------------------------------------------------------------------------------------";
    mostrarEncabezadosProducto();
    for (int i = 0; i < totalProductos; i++){
        if(productos[i].status == 1){
            mostrarInfoProducto(productos[i]);
        }
    }
}

bool altaProducto(string& nombreProducto, float& pc, float& pv, int& existencia, int& nivelReorden){
    productos[totalProductos].id = totalProductos + 1;
    productos[totalProductos].producto = nombreProducto;
    productos[totalProductos].pc = pc;
    productos[totalProductos].pv = pv;
    productos[totalProductos].existencias = existencia;
    productos[totalProductos].nivelReorden = nivelReorden;
    productos[totalProductos].status = 1;
    totalProductos++; // se incrementa en 1 la cantidad de productos.
    return true;
}

Producto* buscarProducto(const string& nombreProducto){
    for(int i = 0; i < totalProductos; i++){
        if(convertirMinus(productos[i].producto) == convertirMinus(nombreProducto)){
            return &productos[i];
        }
    }
    return nullptr;
}

NodoUsuario* buscarUsuario(NodoUsuario*& lista, string nombreUsuario){
    string usuario_aux;
    NodoUsuario* ptr = lista;
    while(ptr != NULL){
        usuario_aux = ptr->usuario.usuario;
        if(convertirMinus(usuario_aux) == convertirMinus(nombreUsuario)){
            return ptr;
        }
        ptr = ptr->next;
    }
    return nullptr;
}

string convertirMinus(string str){
    for (char &c : str) c = tolower(c);
    return str;
}

bool consultarProducto(Producto* producto){
    if(producto == nullptr || producto->status ==0){
        return false;
    }
    mostrarEncabezadosProducto();
    mostrarInfoProducto(*producto);
    return true;
    
}

void mostrarEncabezadosProducto(){
    cout << "\n" << left << setw(5) << "Id"
            << setw(15) << "Producto"
            << setw(12) << "PC"
            << setw(12) << "PV"
            << setw(15) << "Existencias"
            << setw(20) << "Nivel de Reorden"
            << "Resurtir" << endl;
}

void mostrarInfoProducto(const Producto& producto){
    char resurtir;
    resurtir = (producto.existencias <= producto.nivelReorden) ? '*' : ' ';
    cout << left << setw(5) << producto.id
            << setw(15) << producto.producto
            << setw(12) << formatoMoneda(producto.pc)
            << setw(12) << formatoMoneda(producto.pv)
            << setw(15) << producto.existencias
            << setw(20) << producto.nivelReorden
            << resurtir << endl;
}

void menuModificaciones(Producto* producto){
    bool mostrarOpciones = true;
    float pc, pv;
    int existencia, nivelReorden, opcion;
    limpiarConsola();
    while(mostrarOpciones){
            cout << "\n\n\tMODIFICACIONES\n\nProducto: "<< producto->producto << endl;
            mostrarEncabezadosProducto();
            mostrarInfoProducto(*producto);
            cout << "\n1. Precio de compra\n2. Precio de venta\n3. Existencias\n4. Nidel de reorden\n5. Regresar al menu anterior\n\n";
            cout << "\tOpcion: "; cin >> opcion; validarInput();
            switch(opcion){
                case 1:
                    solicitarYActualizarCampoProducto(producto,"Precio de Compra actual: ",producto->pc,"Precio de Compra nuevo: ","pc","Precio de compra actualizado");
                    break;
                case 2:
                    solicitarYActualizarCampoProducto(producto,"Precio de Venta actual: ",producto->pv,"Precio de Venta nuevo: ","pv","Precio de venta actualizado");
                    break;
                case 3:
                    solicitarYActualizarCampoProducto(producto,"Existencias actuales: ",producto->existencias,"Existencias nuevas: ","existencias","Existencias actualizadas");
                    break;
                case 4:
                    solicitarYActualizarCampoProducto(producto,"Nivel de reorden actual: ",producto->nivelReorden,"Nivel de reorden nuevo: ","nivelReorden","Nivel de reorden actualizado");
                    break;
                case 5:
                    limpiarConsola();
                    mostrarOpciones = false;
                    break;
                default:
                    cout << "\n*** Opcion incorrecta. Intenta de nuevo. ***";
                    break;
            }
        }
}

void solicitarYActualizarCampoProducto(Producto* producto, const string&textoActual, float valorActual, const string&textoNuevo, const string& campoModificar, const string& textoExito){
    float nuevoValor;
    cout << "\n" << textoActual << valorActual;
    cout << "\n" << textoNuevo; cin >> nuevoValor; validarInput();
    if(modificarProducto(producto, campoModificar, nuevoValor)){
        limpiarConsola();
        cout << "\n\n"<< textoExito <<"\n\n";
    }else{
        cout << "\n\n*** Error. Intenta de nuevo ***\n\n";
    }
}

bool modificarProducto(Producto* producto, const string& campoModificar, float valor){
    if(valor <= 0){ return false; }

    if(campoModificar == "pc"){
        producto->pc = valor;
        return true;
    }
    if(campoModificar == "pv"){
        producto->pv = valor;
        return true;
    }
    if(campoModificar == "existencias"){
        producto->existencias = valor;
        return true;
    }
    if(campoModificar == "nivelReorden"){
        producto->nivelReorden = valor;
        return true;
    }
    
    return false; // en dado caso que no entre en ninguna opcion.
}

bool bajaProducto(Producto* producto){
    if(producto == nullptr || producto->status == 0){
        return false;
    }
    producto->status = 0;
    return true;
}

void mostrarMenuAdminCuentasUsuario(){
    int option;
    bool ejecutarMenu = true;
    string nombreUsuario;

    while(ejecutarMenu){
        cout << "\n\n\tMENU ADMINISTRACION DE CUENTAS DE USUARIOS\n\n";
        cout << "1. Altas\n2. Bajas\n3. Consultas\n4. Modificaciones\n5. Mostrar cuentas de usuarios\n6. Regresar al menu anterior.\n\n";
        cout << "\tOpcion: ";
        cin >> option;
        validarInput();
        limpiarConsola();
        switch (option){
            case 1:
                while(true){
                    cout << "\n\n\tALTA USUARIO\n\n";
                    cout << "Usuario: "; cin >> nombreUsuario;
                    if (nombreUsuario == "*"){limpiarConsola(); break;}
                    NodoUsuario* nodo = buscarUsuario(listaUsuarios, nombreUsuario);
                    altaUsuario(nodo, nombreUsuario);
                }
                break;
            case 2:
                while(true){
                    cout << "\n\n\tBAJA USUARIO\n\n";
                    cout << "Usuario: "; cin >> nombreUsuario;
                    if (nombreUsuario == "*"){limpiarConsola(); break;}
                    NodoUsuario* nodo = buscarUsuario(listaUsuarios, nombreUsuario); // TODO: validar si es necesario pasar la lista por parametro.
                    bajaUsuario(nodo, nombreUsuario);
                }
                break;
            case 3:
                while(true){
                    cout << "\n\n\tCONSULTA USUARIO\n\n";
                    cout << "Usuario: "; cin >> nombreUsuario;
                    if (nombreUsuario == "*"){limpiarConsola(); break;}
                    NodoUsuario* nodo = buscarUsuario(listaUsuarios, nombreUsuario);
                    consultarUsuario(nodo, nombreUsuario);
                }
                break;
            case 4:
                while(true){
                    cout << "\n\n\tMODIFICACIONES\n\n";
                    cout << "Usuario: "; cin >> nombreUsuario;
                    if (nombreUsuario == "*"){limpiarConsola(); break;}
                    NodoUsuario* nodo = buscarUsuario(listaUsuarios, nombreUsuario);
                    modificarUsuario(nodo, nombreUsuario);
                }
                break;
            case 5:
                mostrarCuentasUsuarios();
                break;
            case 6:
                ejecutarMenu = false;
                break;
            default:
                cout << "\n*** Opcion invalida. Intenta de nuevo. ***";
                break;
        }
    }
}

void altaUsuario(NodoUsuario* nodo, string nombreUsuario){
    string pass;
    int tipoUsuario;
    
    if(nodo == nullptr){
        cout << "Contraseña: "; cin >> pass;
        do{
            cout << "Tipo (1 admin / 2 vendedor): "; cin >> tipoUsuario; validarInput();
            if(tipoUsuario != 1 && tipoUsuario != 2) { cout << "\n\n*** Datos invalidos. Intenta de nuevo. ***\n\n\n"; }
        }while(tipoUsuario != 1 && tipoUsuario != 2);
        
        // se crea el usuario
        Usuario nuevoUsuario;
        nuevoUsuario.usuario = nombreUsuario;
        nuevoUsuario.pass = pass;
        nuevoUsuario.tipo = tipoUsuario;
        nuevoUsuario.status = 1;
        
        // se agrega el usuario.
        agregarUsuarioLista(listaUsuarios, nuevoUsuario);
        totalUsuarios++; // se incrementa en 1 la cantidad de usuarios.
        
        string tipoUsuarioStr = (tipoUsuario == 2) ? "Vendedor" : "Admin";
        cout << "\n\nEl Usuario \"" << nombreUsuario << "\" se agrego correctamente como " << tipoUsuarioStr << ".\n\n";
        return;
    }
    if(nodo->usuario.status == 1){
        cout << "\n\n*** El usuario \"" << nodo->usuario.usuario << "\" ya existe. Intenta de nuevo. ***\n\n"; 
        return;
    }else {
        char res;
        cout << "\nEl Usuario estaba dado de baja, ¿Quieres darlo de alta nuevamente? (y/n): "; cin >> res; cout << '\n';
        if(tolower(res) == 'y'){
            nodo->usuario.status = 1;
            mostrarEncabezadosUsuario();
            mostrarInfoUsuario(nodo->usuario);
            cout << "\n\nEl usuario " << nodo->usuario.usuario << " se dio de alta.\n\n";
        }
    }
}

void bajaUsuario(NodoUsuario* nodo, string nombreUsuario){
    if(nodo == nullptr || nodo->usuario.status == 0){
        cout << "\n\n*** No se encontro el usuario \"" << nombreUsuario << "\" ***\n";
        return;
    }

    char res;
    cout << "\nEstas seguro que quieres darlo de baja? (y/n): "; cin >> res;
    if(tolower(res) != 'y') return;

    if(nodo->usuario.tipo == 1){ // 1 = admin
        if(contarUsuariosAdmin() == 1){
            cout << "\n\n*** Error! Debe existir por lo menos un admin. ***\n\n";
            return;
        }
        cout << "\nEl usuario es admin, por favor vuelve a confirmar (y/n): "; cin >> res;
            if(tolower(res) != 'y') return;
    }

    nodo->usuario.status = 0; 
    cout << "\nEl usuario \"" << nodo->usuario.usuario << "\" se dio de baja\n";
}

int contarUsuariosAdmin(){
    NodoUsuario* ptr = listaUsuarios;
    int totalAdmin = 0;
    while(ptr != NULL){
        if(ptr->usuario.tipo == 1 && ptr->usuario.status == 1){ totalAdmin++; }
        ptr = ptr->next;
    }
    return totalAdmin;
}

void modificarUsuario(NodoUsuario* nodo, string nombreUsuario){
    if(nodo == nullptr || nodo->usuario.status == 0){
        cout << "\n\n***Usuario \"" << nombreUsuario << "\" no encontrado. Intenta de nuevo. ***\n\n";
        return;
    }
    string pass;
    int opcion, tipoUsuario;
    bool mostrarOpciones = true;
    limpiarConsola();
    while(mostrarOpciones){
        cout << "\n\n\tMODIFICACIONES\n\nUsuario: "<< nombreUsuario << endl;
        mostrarEncabezadosUsuario();
        mostrarInfoUsuario(nodo->usuario);
        cout << "\n1. Contraseña\n2. Tipo\n3. Regresar al menu anterior\n\n";
        cout << "\tOpcion: "; cin >> opcion; validarInput();
        switch(opcion){
            case 1:
            cout << "\nContraseña actual: "<< nodo->usuario.pass;
            cout << "\nContraseña nueva: "; cin >> pass;
            nodo->usuario.pass = pass; // actualizamos el password
            limpiarConsola();
            cout << "\n\n\tContraseña actualizada\n\n";
            break;
            case 2:
                do{
                    cout << "\nTipo actual: "<< nodo->usuario.tipo;
                    cout << "\nTipo nuevo: "; cin >> tipoUsuario; validarInput();
                    if(tipoUsuario != 1 && tipoUsuario != 2) { cout << "\n\n*** Datos invalidos. (1 admin / 2 vendedor). Intenta de nuevo. ***\n\n\n"; }
                }while(tipoUsuario != 1 && tipoUsuario != 2);
                // solo se puede modificar el tipo del admin si existen mas de uno
                if(nodo->usuario.tipo == 1 && tipoUsuario == 2){
                    if(contarUsuariosAdmin() == 1){
                        limpiarConsola();
                        cout << "\n\n*** Error! Debe existir por lo menos un admin. ***\n\n";
                        break;
                    }
                }
                nodo->usuario.tipo = tipoUsuario;
                limpiarConsola();
                cout << "\n\n\tTipo de Usuario actualizado\n\n";
                break;
            case 3:
                limpiarConsola();
                mostrarOpciones = false;
                break;
            default:
                limpiarConsola();
                cout << "*** Opcion incorrecta. Intenta de nuevo. ***";
                break;
        }
    }
}

void mostrarEncabezadosUsuario(){
    cout << "\n" << left 
        << setw(15) << "Usuario"
        << setw(10) << "Pass"
        << setw(10) << "Tipo"
        << "St" << endl;
}

void mostrarInfoUsuario(const Usuario& usuario){
    cout << left 
        << setw(15) << usuario.usuario
        << setw(10) << usuario.pass
        << setw(10) << usuario.tipo
        << usuario.status << endl;
}


void consultarUsuario(NodoUsuario* nodo, string nombreUsuario){
    if(nodo == nullptr || nodo->usuario.status == 0){
        cout << "\n\n*** No se encontro el usuario \"" << nombreUsuario << "\" ***\n\n";
        return;
    }
    mostrarEncabezadosUsuario();
    mostrarInfoUsuario(nodo->usuario);
}

void mostrarCuentasUsuarios(){
    cout << "---------------------------------------------------------------------------\n\t\t\tMOSTRAR CUENTAS DE USUARIOS\n---------------------------------------------------------------------------\n";
    mostrarEncabezadosUsuario();
    
    NodoUsuario* ptr = listaUsuarios;
    while(ptr != NULL){
        if(ptr->usuario.status == 1){
            mostrarInfoUsuario(ptr->usuario);
        }
        ptr = ptr->next;
    }
    cout << "\n\n";
}

void hacerVenta(){
    if (validarLogin(2)){
        limpiarConsola();
        Producto* producto;
        Venta ventaActual;
        string nombreProducto;
        int cantidad;
        bool compraRealizada = false;
        
        cout << "\n\n\tVENTAS: " << currentUser <<"\n\n";
        while(true){
            cout << "Producto: "; cin >> nombreProducto;

            if(nombreProducto == "*"){ // imprimir ticket
                limpiarConsola(); 
                ventas[totalVentas] = ventaActual; // agregamos la venta a la lista
                compraRealizada = true;
                imprimirTicket(ventaActual); 
                totalVentas++; // se incrementa en uno las ventas
                reiniciarVenta(ventaActual); // se vuelve a inicializar vacio
                continue;
            }
            if(nombreProducto == "**"){ // corte de caja vendedor
                if(!compraRealizada){ cout << "\n\n*** Finaliza la compra primero con '*' e intenta de nuevo. ***\n\n\n"; continue; } limpiarConsola();
                corteCajaVendedor(currentUser);
                break;
            }

            producto = buscarProducto(nombreProducto);
            // validamos que el producto exista o no este dado de baja
            if(producto == nullptr || producto->status == 0){ cout << "\n\n*** No se encontro el producto. Intenta de nuevo ***\n\n"; continue; }
            // validamos que tengan existencias
            if(producto->existencias <= 0){ cout << "\n\n*** El producto no tiene existencia. Intenta de nuevo ***\n\n"; continue; }

            cout << "Cantidad: "; cin >> cantidad; validarInput();
            if(cantidad <= 0){cout << "\n\n*** Cantidad invalida. Intenta de nuevo ***\n\n"; continue;}

            // validamos que tengamos las existencias que solicita el cliente
            if(producto->existencias < cantidad){
                string resp;
                cout << "\n\nNo hay " << cantidad << ", solo hay " << producto->existencias << ", ¿realizar la venta de " << producto->existencias << "? ('Y' para aceptar / cualquiera para omitir): ";
                cin >> resp;
                if( convertirMinus(resp) != "y") { cout << "\n"; continue; }
                cantidad = producto->existencias;
                cout << "Se agregaron " << producto->existencias << endl;
            }
            
            // creamos la venta dentro de la estructura de Ventas
            ventaActual.vendedor = currentUser; // vendedor loggeado
            ventaActual.productos[ventaActual.totalProductosVentas] = producto->producto; // nombre del producto
            ventaActual.cantidad[ventaActual.totalProductosVentas] = cantidad;
            ventaActual.pc[ventaActual.totalProductosVentas] = producto->pc;
            ventaActual.pv[ventaActual.totalProductosVentas] = producto->pv;
            ventaActual.totalProductosVentas++;

            // Restar producto al inventario
            restarInventario(producto->id, cantidad);

            cout << "\n";
        }
    }
}

void imprimirTicket(const Venta& venta){
    float total = 0;
    cout << "\n\n-------------------------------------------------------\n\n";
    cout << "\t\tAbarrotes \"El Inge\"";
    cout << "\n\t\t Ticket de Venta\n\n\n";
    cout << "Vendedor: " << currentUser << endl;

    cout << "\n" << left << setw(15) << "Producto"
                    << setw(12) << "Cantidad"
                    << setw(20) << "Precio Unitario"
                    << "Subtotal" << endl;
    for(int i = 0; i < venta.totalProductosVentas; i++){
        float subtotal = venta.cantidad[i] * venta.pv[i];
        total += subtotal;
        cout << left << setw(15) << venta.productos[i]
                << setw(12) << venta.cantidad[i]
                << setw(20) << formatoMoneda(venta.pv[i])
                << formatoMoneda(subtotal) << endl;
    }
    cout << "\n\t\t\t\t     Total: $" << total << "\n\n-------------------------------------------------------\n\n";
}

string formatoMoneda(float monto){
    ostringstream oss;
    oss << "$" << fixed << setprecision(2) << monto;
    return oss.str();
}

void reiniciarVenta(Venta& venta) {
    venta.vendedor = "";
    for (int i = 0; i < venta.totalProductosVentas; i++) {
        venta.productos[i] = "";
        venta.cantidad[i] = 0;  
        venta.pc[i] = 0.0;      
        venta.pv[i] = 0.0;
    }
    venta.totalProductosVentas = 0;
}

void restarInventario(const int& id, const int& cantidad){
    for(int i=0; i<totalProductos; i++){
        if(productos[i].id == id){
            productos[i].existencias -= cantidad;
            break;
        }
    }
}

void corteCajaVendedor(const string& vendedor){
    float ingresos = 0, egresos = 0;
    
    cout << "\n\n-------------------------------------------------------\n\n";
    cout << "\t\tCorte de Caja: " << vendedor << "\n\n";
    for(int i=0; i < totalVentas; i++){
        if(ventas[i].vendedor == vendedor){
            for(int k=0; k < ventas[i].totalProductosVentas; k++){
                ingresos += (ventas[i].pv[k] * ventas[i].cantidad[k]);
                egresos  += (ventas[i].pc[k] * ventas[i].cantidad[k]);
            }
        }
    }
    cout << "Ingresos: $" << ingresos << endl;
    cout << "Egresos:  $" << egresos << endl;
    cout << "\nUtilidad: $" << ingresos - egresos << endl;
    cout << "\n-------------------------------------------------------\n\n";
}

void corteCajaGeneral(){
    float ingresos = 0, egresos = 0;
    cout << "\n\n-------------------------------------------------------\n\n";
    cout << "\t\tCorte general\n\n";
    for(int i=0; i<totalVentas; i++){
        for(int j=0; j < ventas[i].totalProductosVentas; j++){
            ingresos += (ventas[i].pv[j] * ventas[i].cantidad[j]);
            egresos  += (ventas[i].pc[j] * ventas[i].cantidad[j]);
        }
    }
    cout << "Ingresos: $" << ingresos << endl;
    cout << "Egresos:  $" << egresos << endl;
    cout << "\nUtilidad: $" << ingresos - egresos << endl;
    cout << "\n-------------------------------------------------------\n\n";
}